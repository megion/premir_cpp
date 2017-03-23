#ifndef SRC_CACHE_WRAPPER_H
#define SRC_CACHE_WRAPPER_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include "utils/console_colors.h"

namespace cache {

	void *xmalloc(size_t size); 
	void *xmallocz(size_t size);
	/*
	 * xmemdupz() allocates (len + 1) bytes of memory, duplicates "len" bytes of
	 * "data" to the allocated memory, zero terminates the allocated memory,
	 * and returns a pointer to the allocated memory. If the allocation fails,
	 * the program dies.
	 */
	void *xmemdupz(const char *data, size_t len);
	
	char *xstrdup(const char *str);
	
	char *xstrndup(const char *str, size_t len);
	
	//__attribute__((format (printf, 3, 4)))
	int xsnprintf(char *dst, size_t max, const char *fmt, ...);

	ssize_t xread(int fd, void *buf, size_t len);
	ssize_t xwrite(int fd, const void *buf, size_t len);
	ssize_t xpread(int fd, void *buf, size_t len, off_t offset);

	/*
	 * Forward decl that will remind us if its twin in cache.h changes.
	 * This function is used in compat/pread.c.  But we can't include
	 * cache.h there.
	 */
	ssize_t read_in_full(int fd, void *buf, size_t count);
	
	ssize_t write_in_full(int fd, const void *buf, size_t count);
	ssize_t pread_in_full(int fd, void *buf, size_t count, off_t offset);

}

#endif
