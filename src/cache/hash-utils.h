#ifndef SRC_CACHE_HASH_UTILS_H
#define SRC_CACHE_HASH_UTILS_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>


#define FNV32_BASE ((unsigned int) 0x811c9dc5)
#define FNV32_PRIME ((unsigned int) 0x01000193)

namespace cache {

	/* FNV-1 functions */

	unsigned int strhash(const char *buf);
	unsigned int strihash(const char *buf);
	unsigned int memhash(const void *buf, size_t len);
	unsigned int memihash(const void *buf, size_t len);

	unsigned int sha1hash(const unsigned char *sha1);
	
}

#endif
