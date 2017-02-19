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


namespace test {
    namespace cache_linked_hash_map {

		struct test_entry {
			cache::hashmap_entry ent;
			/* key and value as two \0-terminated strings */
			char *key;
		};

        void linkedHashMap_test();
    }
}

#endif

