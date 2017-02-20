#ifndef TEST_CACHE_LINKED_HASH_MAP_H
#define TEST_CACHE_LINKED_HASH_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "test.h"

#include "cache/LinkedHashMap.h"
#include "cache/DirCache.h"

namespace test {
    namespace cache_linked_hash_map {

		struct test_entry {
			cache::hashmap_entry ent;
			/* key and value as two \0-terminated strings */
			char key[FLEX_ARRAY];
			test_entry(unsigned int hash) {
				ent.hash = hash;
				ent.next = nullptr;
			}
			// compare function for hash map
			bool operator==(const test_entry &other) const {
				// check same pointer or nullptr
				if(key==other.key) {
					return true;
				}
				return (std::strcmp(key, other.key)==0);
			}
			bool operator!=(const test_entry &other) const {
				return !((*this) == other);
			}
		};

        void linkedHashMap_test();
    }
}

#endif

