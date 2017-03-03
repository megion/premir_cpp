#ifndef SRC_CACHE_WRAPPER_H
#define SRC_CACHE_WRAPPER_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

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

}

#endif
