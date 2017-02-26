#ifndef SRC_CACHE_LINKED_HASH_MAP_H
#define SRC_CACHE_LINKED_HASH_MAP_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include "cache/hash-utils.h"
#include "utils/console_colors.h"

/*
 * Generic implementation of hash-based key-value mappings.
 * See Documentation/technical/api-hashmap.txt.
 */
#define HASHMAP_INITIAL_SIZE 64
/* grow / shrink by 2^2 */
#define HASHMAP_RESIZE_BITS 2
/* load factor in percent */
#define HASHMAP_LOAD_FACTOR 80


namespace cache {
	
	/*
	 * data structures
	 * D is type of data 
	 */
	struct hashmap_entry {
		struct hashmap_entry *next;
		unsigned int hash;
	};

	/*
	 * T type must be entry exdends hashmap_entry, for example:
	   struct pool_entry {
	       struct hashmap_entry ent; // must be first
		   size_t len;
		   unsigned char data[FLEX_ARRAY];
		};

		D type of data. For example may be 'unsigned char' or other structure.
		This data is used for find entry.
	 */
	template<typename T, typename D>
	class LinkedHashMap {
		public:

			class EntryComparator {
				public:
					/*
					 * compare entry with keyHash and keyData
					 */
					virtual bool equals(const T* entry, unsigned int keyHash, const D* keyData) const = 0;
					/*
					 * compare entry1 with entry2
					 */
					virtual bool equals(const T* entry1, const T* entry2) const = 0;
			};

			LinkedHashMap(size_t initial_size, EntryComparator *_comparator) : size(0), comparator(_comparator) {
				unsigned int _size = HASHMAP_INITIAL_SIZE;

				/* calculate initial table size and allocate the table */
				initial_size = (unsigned int) ((uint64_t) initial_size * 100
						/ HASHMAP_LOAD_FACTOR);
				while (initial_size > _size) {
					_size <<= HASHMAP_RESIZE_BITS;
				}
				alloc_table(_size);
			}
			
			/*
			 * Iterator 
			 */
			class MapInterator {
				public:
					// hashmap_iter_init
					MapInterator(LinkedHashMap<T,D>* _map): map(_map), nextEntry(nullptr), tablepos(0) {
					}

					/*
					 * hashmap_iter_next
					 */	
					T *getNext() {
						hashmap_entry *current = (hashmap_entry*)nextEntry;
						for (;;) {
							if (current) {
								nextEntry = (T*)current->next;
								return (T*)current;
							}
							if (tablepos >= map->tablesize) {
								return nullptr;
							}
							current = (hashmap_entry*)(map->table[tablepos++]);
						}
					}

					void reset() {
						tablepos = 0;
						nextEntry = nullptr;
					}

					/* 
					 * hashmap_iter_first
					 */ 
					T *firstIter() {
						reset();
						return getNext();
					}
	
				public:
					T *nextEntry;

				private:
					LinkedHashMap<T,D> *map;
					unsigned int tablepos;
			};

			~LinkedHashMap() {
				freeMap(false);
			}

			/*
			 * hashmap_free
			 */
			void freeMap(bool free_entries) {
				if (!table) {
					return;
				}
				if (free_entries) {
					T *e;
					MapInterator iter(this);
					while ((e = iter.getNext())) {
						std::free(e);
					}
				}
				std::free(table);
				table = nullptr;
				size = 0;
				tablesize = 0;
				//memset(map, 0, sizeof(*map));
			}

			void printMap() const {
				size_t cn = 0;
				for (size_t i = 0; i < tablesize; i++) {
					hashmap_entry *e = (hashmap_entry*)table[i];
					while (e) {
						std::printf("entry %d: tablepos=%d, next=%p, hash=%d\n", cn, i, e->next, e->hash); 
						e = e->next;
						cn++;
					}
				}
			}
						
			void rehash(unsigned int newsize) {
				unsigned int i, oldsize = tablesize;
				T **oldtable = table;

				alloc_table(newsize);
				for (i = 0; i < oldsize; i++) {
					hashmap_entry *e = (hashmap_entry*)oldtable[i];
					while (e) {
						hashmap_entry *next = e->next;
						unsigned int b = bucket(e->hash);
						e->next = (hashmap_entry*)table[b];
						table[b] = (T*)e;
						e = next;
					}
				}
				std::free(oldtable);
			}

			/*
			 * find_entry_ptr
			 */
			T** findEntryPtr(unsigned int hash, const D *keyData) const {
				T** e = &table[bucket(hash)];
				while(*e && !comparator->equals(*e, hash, keyData)) {
					e = (T**)&(((hashmap_entry*)(*e))->next);
				}
				return e;
			}

			/*
			 * find_entry_ptr
			 */
			T** findEntryPtr(const T *key) const {
				T** e = &table[bucket(((hashmap_entry*)key)->hash)];
				while(*e && !comparator->equals(*e, key)) {
					e = (T**)&(((hashmap_entry*)(*e))->next);
				}
				return e;
			}

			/*
			 * hashmap_get or hashmap_get_from_hash 
			 */
			T* getEntry(unsigned int hash, const D *keyData) {
				return *findEntryPtr(hash, keyData);
			}

			/*
			 * hashmap_get or hashmap_get_from_hash 
			 */
			T* getEntry(const T *key) const {
				return *findEntryPtr(key);
			}
		
			/*
			 * hashmap_get_next
			 */	
			T* findNext(const T *key) const {
				hashmap_entry *entry = (hashmap_entry*) key;
				hashmap_entry *ne = entry->next;
				for (; ne; ne = ne->next) {
					if (comparator->equals((T*)ne, key)) {
						return (T*)ne;
					}
				}
				return nullptr;
			}

			/*
			 * hashmap_add
			 * add entry to hash map. User must allocated memory for entry before add
			 */
			void add(T* e) {
				hashmap_entry *entry = (hashmap_entry*) e;
				unsigned int b = bucket(entry->hash);

				/* add entry */
				entry->next = (hashmap_entry*)table[b];
				table[b] = e;

				/* fix size and rehash if appropriate */
				size++;
				if (size > grow_at) {
					rehash(tablesize << HASHMAP_RESIZE_BITS);
				}
			}

			/*
			 * hashmap_remove
			 */
			T* remove(T *key) {
				T **e = findEntryPtr(key);
				if (*e == nullptr) {
					return nullptr;
				}

				/* remove existing entry */
				T *old = *e;
				*e = (T*) (((hashmap_entry*)old)->next);
				((hashmap_entry*)old)->next = nullptr;

				/* fix size and rehash if appropriate */
				size--;
				if (size < shrink_at) {
					rehash(tablesize >> HASHMAP_RESIZE_BITS);
				}
				return old;
			}

			/*
			 * hashmap_put
			 */
			T* put(T* entry) {
				T* old = remove(entry);
				add(entry);
				return old;
			}

			unsigned int getSize() {
				return size;
			}
			
		private:
			T** table;
			unsigned int size, tablesize, grow_at, shrink_at;
			EntryComparator *comparator;

			void alloc_table(unsigned int _size) {
				tablesize = _size;
				table = (T**) std::calloc(tablesize, sizeof(T*));
				if (table == nullptr) {
					throw std::runtime_error(std::strerror(errno));
				}

				/* calculate resize thresholds for new size */
				grow_at = (unsigned int) ((uint64_t) tablesize * HASHMAP_LOAD_FACTOR / 100);
				if (tablesize <= HASHMAP_INITIAL_SIZE) {
					shrink_at = 0;
				} else {
					/*
					 * The shrink-threshold must be slightly smaller than
					 * (grow-threshold / resize-factor) to prevent erratic resizing,
					 * thus we divide by (resize-factor + 1).
					 */
					shrink_at = grow_at / ((1 << HASHMAP_RESIZE_BITS) + 1);
				}
			}	

			unsigned int bucket(unsigned int hash) const {
				return hash & (tablesize - 1);
			}

	};

}

#endif
