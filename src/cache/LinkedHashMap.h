#ifndef SRC_CACHE_HASHMAP_H
#define SRC_CACHE_HASHMAP_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include "cache/DirCache.h"


/*
 * Generic implementation of hash-based key-value mappings.
 * See Documentation/technical/api-hashmap.txt.
 */
#define FNV32_BASE ((unsigned int) 0x811c9dc5)
#define FNV32_PRIME ((unsigned int) 0x01000193)

#define HASHMAP_INITIAL_SIZE 64
/* grow / shrink by 2^2 */
#define HASHMAP_RESIZE_BITS 2
/* load factor in percent */
#define HASHMAP_LOAD_FACTOR 80


namespace cache {
	/* FNV-1 functions */
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

	static inline unsigned int sha1hash(const unsigned char *sha1) {
		/*
		 * Equivalent to 'return *(unsigned int *)sha1;', but safe on
		 * platforms that don't support unaligned reads.
		 */
		unsigned int hash;
		memcpy(&hash, sha1, sizeof(hash));
		return hash;
	}

	/* data structures */
	struct hashmap_entry {
		struct hashmap_entry *next;
		unsigned int hash;
	};


	/* hashmap_entry functions */

	//static void hashmap_entry_init(hashmap_entry *entry, unsigned int hash) {
		//struct hashmap_entry *e = entry;
		//e->hash = hash;
		//e->next = NULL;
	//}
	
	/*
	 * T type must be entry exdends hashmap_entry, for example:
	   struct pool_entry {
	       struct hashmap_entry ent; // must be first
		   size_t len;
		   unsigned char data[FLEX_ARRAY];
		};
	 */
	template<typename T>
	class LinkedHashMap {
		public:
			//typedef int (*hashmap_cmp_fn)(const hashmap_entry<T> *entry,
					   //const hashmap_entry<T> *entry_or_key, const void *keydata);
			
			LinkedHashMap(size_t initial_size) : size(0) {
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
					MapInterator(LinkedHashMap<T>* _map): map(_map), nextEntry(nullptr), tablepos(0) {
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
					LinkedHashMap<T> *map;
					unsigned int tablepos;
			};

			~LinkedHashMap() {
				hashmap_free(true);
			}

			void hashmap_free(bool free_entries) {
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

			bool entry_equals(const T *e1, const T *e2) {
				hashmap_entry* entry1 = (hashmap_entry*) e1;
				hashmap_entry* entry2 = (hashmap_entry*) e2;
				return (e1 == e2) || (entry1->hash == entry2->hash && *e1 == *e2);
			}

			unsigned int bucket(unsigned int hash) {
				return hash & (tablesize - 1);
			}
			
			void rehash(unsigned int newsize) {
				unsigned int i, oldsize = tablesize;
				T **oldtable = table;

				alloc_table(newsize);
				for (i = 0; i < oldsize; i++) {
					T *entry = oldtable[i];
					while (e) {
						hashmap_entry *e = (hashmap_entry*) entry;
						hashmap_entry *next = e->next;
						unsigned int b = bucket(e->hash);
						e->next = table[b];
						table[b] = e;
						e = next;
					}
				}
				std::free(oldtable);
			}

			

			/*
			 * find_entry_ptr
			 * You must set hash field in key pointer. Other fields are optional.
			 * The each entries compare by == operator
			 */
			T** findEntryPtr(const T *key) {
				const hashmap_entry *keyEntry = (hashmap_entry*) key;
				T** e = &table[bucket(keyEntry->hash)];
				while(*e && !entry_equals(*e, key)) {
					e = &( (T*) (((hashmap_entry*)(*e))->next));
				}
				return e;
			}

			/*
			 * hashmap_get or hashmap_get_from_hash 
			 */
			T* getEntry(const T *key) {
				return *findEntryPtr(key);
			}
		
			/*
			 * hashmap_get_next
			 */	
			T* findNext(const T *key) {
				hashmap_entry *entry = (hashmap_entry*) key;
				hashmap_entry *ne = entry->next;
				for (; ne; ne = ne->next) {
					if (entry_equals(key, (T*)ne)) {
						return (T*)ne;
					}
				}
				return nullptr;
			}

			/*
			 * hashmap_add
			 * add entry to hash map. User must allocated memory for entry before add
			 */
			void add(const T* e) {
				hashmap_entry *entry = (hashmap_entry*) e;
				unsigned int b = bucket(entry->hash);

				/* add entry */
				entry->next = table[b];
				table[b] = entry;

				/* fix size and rehash if appropriate */
				size++;
				if (size > grow_at) {
					rehash(tablesize << HASHMAP_RESIZE_BITS);
				}
			}

			/*
			 * hashmap_remove
			 */
			T* remove(const T *key) {
				T *old;
				T **e = findEntryPtr(key);
				if (*e == nullptr) {
					return nullptr;
				}

				/* remove existing entry */
				old = *e;
				*e = (T*) (((hashmap_entry*)old)->next);
				old->next = nullptr;

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
			T* put(const T* entry) {
				T* old = remove(entry);
				add(entry);
				return old;
			}

			
		private:
			T** table;
			unsigned int size, tablesize, grow_at, shrink_at;

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
			
			

	};

	
	/* string interning */
	struct pool_entry {
		size_t len;
		unsigned char data[FLEX_ARRAY];
	};

	static int pool_entry_cmp(const struct hashmap_entry<pool_entry> *e1,
			const struct hashmap_entry<pool_entry> *e2,
			const unsigned char *keydata) {
		return e1->value.data != keydata &&
			(e1->value.len != e2->value.len || memcmp(e1->value.data, keydata, e1->value.len));
	}

	const void *memintern(const void *data, size_t len) {
		// global static string pool 
		/* initialize string pool hashmap */
		static LinkedHashMap<pool_entry> map(pool_entry_cmp, 0);

		//if (!map.tablesize)
			//hashmap_init(&map, (hashmap_cmp_fn) pool_entry_cmp, 0);

		/* lookup interned string in pool */
		hashmap_entry<pool_entry> key(memhash(data, len)); 
		//hashmap_entry_init(&key, memhash(data, len));
		key.value.len = len;
		hashmap_entry<pool_entry>* e = map.getEntry(&key, data);
		if (!e) {
			/* not found: create it */
			//FLEX_ALLOC_MEM(e, data, data, len);
			hashmap_entry_init(e, key.hash);
			e->len = len;
			map.hashmap_add(e);
		}
		return e->data;
	}

	static inline const char *strintern(const char *string) {
		return memintern(string, strlen(string));
	}

}





#endif
