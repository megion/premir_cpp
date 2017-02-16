#include "cache/hash-utils.h"

namespace cache {
	
	unsigned int strhash(const char *str) {
		unsigned int c, hash = FNV32_BASE;
		while ((c = (unsigned char) *str++))
			hash = (hash * FNV32_PRIME) ^ c;
		return hash;
	}

	unsigned int strihash(const char *str) {
		unsigned int c, hash = FNV32_BASE;
		while ((c = (unsigned char) *str++)) {
			if (c >= 'a' && c <= 'z')
				c -= 'a' - 'A';
			hash = (hash * FNV32_PRIME) ^ c;
		}
		return hash;
	}

	unsigned int memhash(const void *buf, size_t len) {
		unsigned int hash = FNV32_BASE;
		unsigned char *ucbuf = (unsigned char *) buf;
		while (len--) {
			unsigned int c = *ucbuf++;
			hash = (hash * FNV32_PRIME) ^ c;
		}
		return hash;
	}

	unsigned int memihash(const void *buf, size_t len) {
		unsigned int hash = FNV32_BASE;
		unsigned char *ucbuf = (unsigned char *) buf;
		while (len--) {
			unsigned int c = *ucbuf++;
			if (c >= 'a' && c <= 'z')
				c -= 'a' - 'A';
			hash = (hash * FNV32_PRIME) ^ c;
		}
		return hash;
	}

	unsigned int sha1hash(const unsigned char *sha1) {
		/*
		 * Equivalent to 'return *(unsigned int *)sha1;', but safe on
		 * platforms that don't support unaligned reads.
		 */
		unsigned int hash;
		memcpy(&hash, sha1, sizeof(hash));
		return hash;
	}

}


