#include "cache/wrapper.h"

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

}


