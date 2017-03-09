#ifndef SRC_CACHE_STRBUF_H
#define SRC_CACHE_STRBUF_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

namespace cache {

	bool starts_with(const char *str, const char *prefix);

}

#endif
