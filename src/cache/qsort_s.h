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
	
	int my_qsort_s(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *), void *ctx);
		
	int qsort_s(void *base, size_t nmemb,
		int (*compar)(const void *, const void *, void *), void *ctx);
}

#endif
