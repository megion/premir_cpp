#ifndef SRC_CACHE_STR_INTERN_H
#define SRC_CACHE_STR_INTERN_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include "cache/LinkedHashMap.h"
#include "cache/DirCache.h"

namespace cache {
	
	/* string interning */
	struct pool_entry {
		struct hashmap_entry ent; // must be first
		size_t len;
		unsigned char *data;

		pool_entry(unsigned int hash) {
			ent.hash = hash;
			ent.next = nullptr;
		}
		// compare function for hash map
		bool operator==(const pool_entry &other) const {
			// check same pointer or nullptr
			if(data==other.data) {
				return true;
			}
			return (len==other.len && (std::memcmp(data, other.data, len)==0));
		}
		bool operator!=(const pool_entry &other) const {
			return !((*this) == other);
		}
	};

	
	
	const void *memintern(const void *data, size_t len); 
	const char *strintern(const char *str);
}

#endif
