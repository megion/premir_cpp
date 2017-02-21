#include "cache/LinkedHashMapTest.h"

namespace test {
	namespace cache_linked_hash_map {
		
		const char *get_value(const test_entry *e) {
			return e->key + std::strlen(e->key) + 1;
		}

		test_entry *alloc_test_entry(int hash, const char *key, int klen, const char *value, int vlen) {
			test_entry *entry = (test_entry*)std::malloc(sizeof(test_entry) + klen + vlen + 2);
			if (entry == nullptr) {
				throw std::runtime_error(std::strerror(errno));
			}
			entry->ent.hash = hash;
			entry->ent.next = nullptr;
			std::memcpy(entry->key, key, klen + 1);
			std::memcpy(entry->key + klen + 1, value, vlen + 1);
			return entry;
		}

		static const unsigned int HASH_METHOD_FNV = 0;
		static const unsigned int HASH_METHOD_I = 1;
		static const unsigned int HASH_METHOD_IDIV10 = 2;
		static const unsigned int HASH_METHOD_0 = 3;
		static const unsigned int HASH_METHOD_X2 = 4;

		unsigned int hash(unsigned int method, unsigned int i, const char *key) {
			unsigned int hash = 0;
			switch (method & 3) {
				case HASH_METHOD_FNV:
					hash = cache::strhash(key);
					break;
				case HASH_METHOD_I:
					hash = i;
					break;
				case HASH_METHOD_IDIV10:
					hash = i / 10;
					break;
				case HASH_METHOD_0:
					hash = 0;
					break;
			}

			if (method & HASH_METHOD_X2)
				hash = 2 * hash;
			return hash;
		}

		/*
		 * Test performance of hashmap.[ch]
		 */
		void perf_hashmap(unsigned int method, unsigned int rounds) {
			char buf[16];

			const unsigned int TEST_SIZE = 16;
			test_entry **entries = (test_entry**)std::malloc(TEST_SIZE * sizeof(test_entry *));
			unsigned int *hashes = (unsigned int*)std::malloc(TEST_SIZE * sizeof(unsigned int));
			cache::LinkedHashMap<test_entry> allValuesMap(0);
			for (unsigned int i = 0; i < TEST_SIZE; i++) {
				snprintf(buf, sizeof(buf), "%i", i);
				test_entry *entry = alloc_test_entry(0, buf, strlen(buf), "", 0);
				unsigned int h = hash(method, i, entry->key);
				entry->ent.hash = h;
				hashes[i] = h;
				entry->ent.next = nullptr;
				entries[i] = entry;
				allValuesMap.add(entry);
			}
			
			/* test adding to the map */

			const unsigned int TEST_ADD = 8;
			for (unsigned int j = 0; j < rounds; j++) {
				cache::LinkedHashMap<test_entry> map(0);

				/* add entries */
				for (unsigned int i = 0; i < TEST_SIZE; i++) {
					test_entry *entry = entries[i];
					if(i < TEST_ADD) {
						entry->ent.next = nullptr;
						map.add(entry);

						test_entry *entry2 = map.getEntry(entry);
						assert(entry2 != nullptr);
						assert(entry2->ent.hash == hashes[i]);
					} else {
						test_entry *entry2 = map.getEntry(entry);
						assert(entry2 == nullptr);
					}
				}
			}

			/* free all entries */
			allValuesMap.freeMap(true);
		}

		void test_perfomance_map() {
			perf_hashmap(HASH_METHOD_FNV, 3);
		}
		
		void test_add_value() {
			const char *p1 = "abcd";
			int l1 = std::strlen(p1);

			const char *p2 = "qwertyuiop";
			int l2 = std::strlen(p2);


			cache::LinkedHashMap<test_entry> map(0);
			map.add(alloc_test_entry(cache::strhash(p1), p1, l1, p2, l2));
			assert(map.getSize() == 1);

			map.add(alloc_test_entry(cache::strhash(p1), p1, l1, p2, l2));
			assert(map.getSize() == 2);

			test_entry *old = map.put(alloc_test_entry(cache::strhash(p1), p1, l1, p2, l2));
			assert(old != nullptr);
			std::free(old);
			assert(map.getSize() == 2);

			// try remove
			test_entry *lookupEntry = alloc_test_entry(cache::strhash(p1), p1, l1, p2, l2);
			test_entry *removed = map.remove(lookupEntry);
			assert(removed != nullptr);
			assert(map.getSize() == 1);
			std::free(removed);
			std::free(lookupEntry);

			map.freeMap(true);
		}

		void test_map_iteration() {
			const char *p1 = "abcd";
			int l1 = std::strlen(p1);
			unsigned int h1 = cache::strhash(p1);

			cache::LinkedHashMap<test_entry> map(0);
			map.add(alloc_test_entry(h1, p1, l1, "", 0));
			map.add(alloc_test_entry(h1, p1, l1, "", 0));
			map.add(alloc_test_entry(h1, p1, l1, "", 0));
			map.add(alloc_test_entry(h1, p1, l1, "", 0));
			assert(map.getSize() == 4);

			const char *p2 = "abcdqerty";
			int l2 = std::strlen(p1);
			unsigned int h2 = cache::strhash(p2);
			map.add(alloc_test_entry(h2, p2, l2, "", 0));
			map.add(alloc_test_entry(h2, p2, l2, "", 0));
			map.add(alloc_test_entry(h2, p2, l2, "", 0));
			map.add(alloc_test_entry(h2, p2, l2, "", 0));
			map.add(alloc_test_entry(h2, p2, l2, "", 0));
			assert(map.getSize() == 9);

			//map.printMap();

			test_entry *lookupEntry = alloc_test_entry(cache::strhash(p1), p1, l1, "", 0);
			test_entry *e = map.getEntry(lookupEntry);
			assert(e != nullptr);
			size_t iterCount = 0;
			while(e) {
				e = map.findNext(e);
				iterCount++;
			}
			assert(iterCount == 4);
			std::free(lookupEntry);

			lookupEntry = alloc_test_entry(cache::strhash(p2), p2, l2, "", 0);
			e = map.getEntry(lookupEntry);
			assert(e != nullptr);
			iterCount = 0;
			while(e) {
				e = map.findNext(e);
				iterCount++;
			}
			assert(iterCount == 5);
			std::free(lookupEntry);

			// iterate by all values
			cache::LinkedHashMap<test_entry>::MapInterator iter(&map);
			iterCount = 0;
			while ((e = iter.getNext())) {
				iterCount++;
			}
			assert(iterCount == 9);

			map.freeMap(true);
		}

		void test_string_interning() {
			//const char *i1 = strintern(p1);
			//const char *i2 = strintern(p1);
			//if (strcmp(i1, p1))
				//printf("strintern(%s) returns %s\n", p1, i1);
			//else if (i1 == p1)
				//printf("strintern(%s) returns input pointer\n", p1);
			//else if (i1 != i2)
				//printf("strintern(%s) != strintern(%s)", i1, i2);
			//else
				//printf("%s\n", i1);
		}

		void linkedHashMap_test() {
			suite("LinkedHashMap");
			mytest(perfomance_map);
			mytest(add_value);
			mytest(map_iteration);
			mytest(string_interning);
		}

	}
}

