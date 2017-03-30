#ifndef SRC_CACHE_SHA1_SHA1_NAME_H
#define SRC_CACHE_SHA1_SHA1_NAME_H

#include <stdio.h>
#include <iostream>

/* The length in bytes and in hex digits of an object name (SHA-1 value). */
#define GIT_SHA1_RAWSZ 20
#define GIT_SHA1_HEXSZ (2 * GIT_SHA1_RAWSZ)


namespace cache {
	namespace sha1 {

		struct object_id {
			unsigned char hash[GIT_SHA1_RAWSZ];
		};
		
		/*
		 * Return an abbreviated sha1 unique within this repository's object database.
		 * The result will be at least `len` characters long, and will be NUL
		 * terminated.
		 *
		 * The non-`_r` version returns a static buffer which remains valid until 4
		 * more calls to find_unique_abbrev are made.
		 *
		 * The `_r` variant writes to a buffer supplied by the caller, which must be at
		 * least `GIT_SHA1_HEXSZ + 1` bytes. The return value is the number of bytes
		 * written (excluding the NUL terminator).
		 *
		 * Note that while this version avoids the static buffer, it is not fully
		 * reentrant, as it calls into other non-reentrant git code.
		 */
		extern const char *find_unique_abbrev(const unsigned char *sha1, int len);
		extern int find_unique_abbrev_r(char *hex, const unsigned char *sha1, int len);
		

	}

}


#endif
