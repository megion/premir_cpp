#include "cache/wrapper.h"

/*
 * Limit size of IO chunks, because huge chunks only cause pain.  OS X
 * 64-bit is buggy, returning EINVAL if len >= INT_MAX; and even in
 * the absence of bugs, large chunks can result in bad latencies when
 * you decide to kill the process.
 *
 * We pick 8 MiB as our default, but if the platform defines SSIZE_MAX
 * that is smaller than that, clip it to SSIZE_MAX, as a call to
 * read(2) or write(2) larger than that is allowed to fail.  As the last
 * resort, we allow a port to pass via CFLAGS e.g. "-DMAX_IO_SIZE=value"
 * to override this, if the definition of SSIZE_MAX given by the platform
 * is broken.
 */
#ifndef MAX_IO_SIZE
# define MAX_IO_SIZE_DEFAULT (8*1024*1024)
# if defined(SSIZE_MAX) && (SSIZE_MAX < MAX_IO_SIZE_DEFAULT)
#  define MAX_IO_SIZE SSIZE_MAX
# else
#  define MAX_IO_SIZE MAX_IO_SIZE_DEFAULT
# endif
#endif


namespace cache {
	
	void *xmalloc(size_t size) {
		void *ret= std::malloc(size);
		if (ret == nullptr) {
			throw std::runtime_error(std::strerror(errno));
		}
		return ret;
	}

	void *xmallocz(size_t size) {
		void *ret = xmalloc(size + 1);
		((char*)ret)[size] = 0;
		return ret;
	}

	/*
	 * xmemdupz() allocates (len + 1) bytes of memory, duplicates "len" bytes of
	 * "data" to the allocated memory, zero terminates the allocated memory,
	 * and returns a pointer to the allocated memory. If the allocation fails,
	 * the program dies.
	 */
	void *xmemdupz(const char *data, size_t len) {
		return std::memcpy(xmallocz(len), data, len);
	}

	char *xstrdup(const char *str) {
		char *ret = strdup(str);
		if (!ret) {
			throw std::runtime_error(std::strerror(errno));
		}
		return ret;
	}

	char *xstrndup(const char *str, size_t len) {
		const char *p = (const char *) std::memchr(str, '\0', len);
		return (char *)xmemdupz(str, p ? p - str : len);
	}

	int xsnprintf(char *dst, size_t max, const char *fmt, ...) {
		va_list ap;
		int len;

		va_start(ap, fmt);
		len = vsnprintf(dst, max, fmt, ap);
		va_end(ap);

		if (len < 0) {
			LOG(ERR, "BUG: your snprintf is broken");
		}
		if (len >= max) {
			LOG(ERR, "BUG: attempt to snprintf into too-small buffer");
		}
		return len;
	}

	static int handle_nonblock(int fd, short poll_events, int err) {
		struct pollfd pfd;

		if (err != EAGAIN && err != EWOULDBLOCK) {
			return 0;
		}

		pfd.fd = fd;
		pfd.events = poll_events;

		/*
		 * no need to check for errors, here;
		 * a subsequent read/write will detect unrecoverable errors
		 */
		poll(&pfd, 1, -1);
		return 1;
	}

	/*
	 * xread() is the same a read(), but it automatically restarts read()
	 * operations with a recoverable error (EAGAIN and EINTR). xread()
	 * DOES NOT GUARANTEE that "len" bytes is read even if the data is available.
	 */
	ssize_t xread(int fd, void *buf, size_t len) {
		ssize_t nr;
		if (len > MAX_IO_SIZE) {
			len = MAX_IO_SIZE;
		}
		while (1) {
			nr = read(fd, buf, len);
			if (nr < 0) {
				if (errno == EINTR) {
					continue;
				}
				if (handle_nonblock(fd, POLLIN, errno))
					continue;
			}
			return nr;
		}
	}

	/*
	 * xwrite() is the same a write(), but it automatically restarts write()
	 * operations with a recoverable error (EAGAIN and EINTR). xwrite() DOES NOT
	 * GUARANTEE that "len" bytes is written even if the operation is successful.
	 */
	ssize_t xwrite(int fd, const void *buf, size_t len) {
		ssize_t nr;
		if (len > MAX_IO_SIZE) {
			len = MAX_IO_SIZE;
		}
		while (1) {
			nr = write(fd, buf, len);
			if (nr < 0) {
				if (errno == EINTR) {
					continue;
				}
				if (handle_nonblock(fd, POLLOUT, errno)) {
					continue;
				}
			}

			return nr;
		}
	}

	/*
	 * xpread() is the same as pread(), but it automatically restarts pread()
	 * operations with a recoverable error (EAGAIN and EINTR). xpread() DOES
	 * NOT GUARANTEE that "len" bytes is read even if the data is available.
	 */
	ssize_t xpread(int fd, void *buf, size_t len, off_t offset) {
		ssize_t nr;
		if (len > MAX_IO_SIZE) {
			len = MAX_IO_SIZE;
		}
		while (1) {
			nr = pread(fd, buf, len, offset);
			if ((nr < 0) && (errno == EAGAIN || errno == EINTR)) {
				continue;
			}
			return nr;
		}
	}

	ssize_t read_in_full(int fd, void *buf, size_t count) {
		char *p = (char*)buf;
		ssize_t total = 0;

		while (count > 0) {
			ssize_t loaded = xread(fd, p, count);
			if (loaded < 0) {
				return -1;
			}
			if (loaded == 0) {
				return total;
			}
			count -= loaded;
			p += loaded;
			total += loaded;
		}

		return total;
	}

	ssize_t write_in_full(int fd, const void *buf, size_t count) {
		const char *p = (const char *)buf;
		ssize_t total = 0;

		while (count > 0) {
			ssize_t written = xwrite(fd, p, count);
			if (written < 0) {
				return -1;
			}
			if (!written) {
				errno = ENOSPC;
				return -1;
			}
			count -= written;
			p += written;
			total += written;
		}

		return total;
	}

	ssize_t pread_in_full(int fd, void *buf, size_t count, off_t offset) {
		char *p = (char *)buf;
		ssize_t total = 0;

		while (count > 0) {
			ssize_t loaded = xpread(fd, p, count, offset);
			if (loaded < 0) {
				return -1;
			}
			if (loaded == 0) {
				return total;
			}
			count -= loaded;
			p += loaded;
			total += loaded;
			offset += loaded;
		}

		return total;
	}

}


