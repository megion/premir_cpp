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
#include "cache/str-intern.h"

namespace test {
    namespace cache_linked_hash_map {

		struct test_entry_data {
			char *key;
		};

		struct test_entry {
			cache::hashmap_entry ent;
			/* key and value as two \0-terminated strings */
			char key[FLEX_ARRAY];
			test_entry(unsigned int hash) {
				ent.hash = hash;
				ent.next = nullptr;
			}
		};

		class TestEntryComparator: public cache::LinkedHashMap<test_entry, test_entry_data>::EntryComparator {
			public:
				bool equals(const test_entry* entry, unsigned int keyHash, const test_entry_data* keyData) const {
					return (entry->ent.hash == keyHash && 
							(std::strcmp(entry->key, keyData->key)==0));
				}
				bool equals(const test_entry* entry1, const test_entry* entry2) const {
					return (entry1->ent.hash == entry2->ent.hash  && 
							(std::strcmp(entry1->key, entry2->key)==0));
				}	
		};

        void linkedHashMap_test();
    }
}

#endif

