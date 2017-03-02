#ifndef SRC_CACHE_Q_SORT_S_H
#define SRC_CACHE_Q_SORT_S_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

namespace cache {

static void *do_xmalloc(size_t size, int gentle)
{
	void *ret;

	if (memory_limit_check(size, gentle))
		return NULL;
	ret = malloc(size);
	if (!ret && !size)
		ret = malloc(1);
	if (!ret) {
		try_to_free_routine(size);
		ret = malloc(size);
		if (!ret && !size)
			ret = malloc(1);
		if (!ret) {
			if (!gentle)
				die("Out of memory, malloc failed (tried to allocate %lu bytes)",
				    (unsigned long)size);
			else {
				error("Out of memory, malloc failed (tried to allocate %lu bytes)",
				      (unsigned long)size);
				return NULL;
			}
		}
	}
#ifdef XMALLOC_POISON
	memset(ret, 0xA5, size);
#endif
	return ret;
}

void *xmalloc(size_t size)
{
	return do_xmalloc(size, 0);
}
static void *do_xmallocz(size_t size, int gentle)
{
	void *ret;
	if (unsigned_add_overflows(size, 1)) {
		if (gentle) {
			error("Data too large to fit into virtual memory space.");
			return NULL;
		} else
			die("Data too large to fit into virtual memory space.");
	}
	ret = do_xmalloc(size + 1, gentle);
	if (ret)
		((char*)ret)[size] = 0;
	return ret;
}

void *xmallocz(size_t size)
{
	return do_xmallocz(size, 0);
}
/*
 * xmemdupz() allocates (len + 1) bytes of memory, duplicates "len" bytes of
 * "data" to the allocated memory, zero terminates the allocated memory,
 * and returns a pointer to the allocated memory. If the allocation fails,
 * the program dies.
 */
void *xmemdupz(const void *data, size_t len)
{
	return memcpy(xmallocz(len), data, len);
}
	int my_qsort_s(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *), void *ctx);
		
	int qsort_s(void *base, size_t nmemb,
		int (*compar)(const void *, const void *, void *), void *ctx);
}

#endif
