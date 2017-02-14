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
	template<typename T>
	struct hashmap_entry {
		hashmap_entry(unsigned int _hash): hash(_hash), next(nullptr) {
		}

		struct hashmap_entry<T> *next;
		unsigned int hash;
		T value;
	};

	

	/* hashmap_entry functions */

	//static void hashmap_entry_init(hashmap_entry *entry, unsigned int hash) {
		//struct hashmap_entry *e = entry;
		//e->hash = hash;
		//e->next = NULL;
	//}
	

	template<typename T>
	class LinkedHashMap {
		public:
			typedef int (*hashmap_cmp_fn)(const hashmap_entry<T> *entry,
				   	const hashmap_entry<T> *entry_or_key, const void *keydata);
			
			LinkedHashMap(hashmap_cmp_fn equals_function, size_t initial_size) {
				unsigned int size = HASHMAP_INITIAL_SIZE;
				size = 0;
				cmpfn = equals_function ? equals_function : always_equal;

				/* calculate initial table size and allocate the table */
				initial_size = (unsigned int) ((uint64_t) initial_size * 100
						/ HASHMAP_LOAD_FACTOR);
				while (initial_size > size) {
					size <<= HASHMAP_RESIZE_BITS;
				}
				alloc_table(size);
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
					hashmap_entry<T> *getNext() {
						hashmap_entry<T> *current = nextEntry;
						for (;;) {
							if (current) {
								nextEntry = current->next;
								return current;
							}

							if (tablepos >= map->tablesize) {
								return nullptr;
							}

							current = map->table[tablepos++];
						}
					}

					void reset() {
						tablepos = 0;
						next = nullptr;
					}

					/* 
					 * hashmap_iter_first
					 */ 
					hashmap_entry<T> *firstIter() {
						reset();
						return getNext();
					}
	
				public:
					hashmap_entry<T> *nextEntry;

				private:
					LinkedHashMap<T> *map;
					unsigned int tablepos;
			};

			~LinkedHashMap() {
				hashmap_free(false);
			}

			void hashmap_free(bool free_entries) {
				if (!table) {
					return;
				}
				if (free_entries) {
					hashmap_entry<T> *e;
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

			int entry_equals(const hashmap_entry<T> *e1, const struct hashmap_entry<T> *e2,
					const void *keydata) {
				return (e1 == e2) || (e1->hash == e2->hash && !cmpfn(e1, e2, keydata));
			}

			unsigned int bucket(unsigned int hash) {
				return hash & (tablesize - 1);
			}
			
			void rehash(unsigned int newsize) {
				unsigned int i, oldsize = tablesize;
				hashmap_entry<T> **oldtable = table;

				alloc_table(newsize);
				for (i = 0; i < oldsize; i++) {
					struct hashmap_entry<T> *e = oldtable[i];
					while (e) {
						hashmap_ent<T> *next = e->next;
						unsigned int b = bucket(e->hash);
						e->next = table[b];
						table[b] = e;
						e = next;
					}
				}
				std::free(oldtable);
			}

			hashmap_entry<T> **find_entry_ptr(const hashmap_entry<T> *key, const void *keydata) {
				hashmap_entry<T> **e = &table[bucket(key->hash)];
				while (*e && !entry_equals(*e, key, keydata)) {
					e = &(*e)->next;
				}
				return e;
			}

			/*
			 * hashmap_get
			 */
			hashmap_entry<T> *getEntry(const hashmap_entry<T> *key, const void *keydata) {
				return *find_entry_ptr(key, keydata);
			}

			hashmap_entry<T> *hashmap_get_from_hash(unsigned int hash, const void *keydata) {
				hashmap_entry<T> key;
				key.hash = hash;
				key.next = nullptr;
				return getEntry(&key, keydata);
			}

			hashmap_entry<T> *hashmap_get_next(const hashmap_entry<T> *entry) {
				hashmap_entry<T> *e = entry->next;
				for (; e; e = e->next) {
					if (entry_equals(entry, e, nullptr)) {
						return e;
					}
				}
				return nullptr;
			}

			/*
			 * hashmap_add
			 */
			void add(unsigned int hash, const T& value) {
				unsigned int b = bucket(hash);

				hashmap_entry<T>* entry = (hashmap_entry<T>*) std::malloc(sizeof(hashmap_entry));
                if (entry == NULL) {
                    throw std::runtime_error(std::strerror(errno));
                }
				entry->hash = hash;
				std::memcpy(entry->value, arr, amount);

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
			hashmap_entry<T>* remove(const hashmap_entry<T> *key, const void *keydata) {
				hashmap_entry<T> *old;
				hashmap_entry<T> **e = find_entry_ptr(key, keydata);
				if (!*e) {
					return nullptr;
				}

				/* remove existing entry */
				old = *e;
				*e = old->next;
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
			hashmap_entry<T>* put(hashmap_entry<T>* entry) {
				hashmap_entry<T>* old = remove(entry, nullptr);
				add(entry);
				return old;
			}

			

			static int always_equal(const void *unused1, const void *unused2, const void *unused3) {
				return 0;
			}
		
		private:
			hashmap_entry<T> **table;
			hashmap_cmp_fn cmpfn;
			unsigned int size, tablesize, grow_at, shrink_at;

			void alloc_table(unsigned int _size) {
				tablesize = _size;
				table = (hashmap_entry<T> **) std::calloc(tablesize, sizeof(struct hashmap_entry<T> *));
				if (table == NULL) {
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
