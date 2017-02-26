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
	
	struct pool_entry_data {
		size_t len;
		unsigned char *data;
	};

	/* string interning */
	struct pool_entry {
		hashmap_entry ent; // must be first
		size_t len;
		unsigned char data[FLEX_ARRAY];

		pool_entry(unsigned int hash) {
			ent.hash = hash;
			ent.next = nullptr;
		}
	};

	class PoolEntryComparator: public LinkedHashMap<pool_entry,pool_entry_data>::EntryComparator {
		public:
			/*
			 * compare entry with hash and data
			 */
			bool equals(const pool_entry* entry, unsigned int keyHash, const pool_entry_data* keyData) const {
				// check same pointer or nullptr
				if(entry->data==keyData->data) {
					return true;
				}
				return (entry->ent.hash == keyHash && entry->len==keyData->len && 
						(std::memcmp(entry->data, keyData->data, entry->len)==0));
			}
			/*
			 * compare entry1 with entry2
			 */
			bool equals(const pool_entry* entry1, const pool_entry* entry2) const {
				// check same pointer or nullptr
				if(entry1->data==entry2->data) {
					return true;
				}
				return (entry1->ent.hash == entry2->ent.hash && entry1->len == entry2->len && 
						(std::memcmp(entry1->data, entry2->data, entry1->len)==0));

			}	
	};	
	
	const void *memintern(const void *data, size_t len); 
	const char *strintern(const char *str);
}

#endif
