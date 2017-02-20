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
			//unsigned int *hashes = (unsigned int*)std::malloc(TEST_SIZE * sizeof(int));
			for (unsigned int i = 0; i < TEST_SIZE; i++) {
				snprintf(buf, sizeof(buf), "%i", i);
				test_entry *entry = alloc_test_entry(0, buf, strlen(buf), "", 0);
				entry->ent.hash = hash(method, i, entry->key);
				entry->ent.next = nullptr;
				entries[i] = entry;
				//hashes[i] = hash(method, i, entries[i]->key);
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
						assert(entry2->ent.hash == entry->ent.hash);
					} else {
						test_entry *entry2 = map.getEntry(entry);
						assert(entry2 == nullptr);
					}
				}
			}
		}

		void test_read_index() {
			perf_hashmap(HASH_METHOD_FNV, 3);
		}

#define DELIM " \t\r\n"

/*
 * Read stdin line by line and print result of commands to stdout:
 *
 * hash key -> strhash(key) memhash(key) strihash(key) memihash(key)
 * put key value -> NULL / old value
 * get key -> NULL / value
 * remove key -> NULL / old value
 * iterate -> key1 value1\nkey2 value2\n...
 * size -> tablesize numentries
 *
 * perfhashmap method rounds -> test hashmap.[ch] performance
 */
//int cmd_main(int argc, const char **argv)
//{
	//char line[1024];
	//struct hashmap map;
	//int icase;

	//[> init hash map <]
	//icase = argc > 1 && !strcmp("ignorecase", argv[1]);
	//hashmap_init(&map, (hashmap_cmp_fn) (icase ? test_entry_cmp_icase
			//: test_entry_cmp), 0);

	//[> process commands from stdin <]
	//while (fgets(line, sizeof(line), stdin)) {
		//char *cmd, *p1 = NULL, *p2 = NULL;
		//int l1 = 0, l2 = 0, hash = 0;
		//struct test_entry *entry;

		//[> break line into command and up to two parameters <]
		//cmd = strtok(line, DELIM);
		//[> ignore empty lines <]
		//if (!cmd || *cmd == '#')
			//continue;

		//p1 = strtok(NULL, DELIM);
		//if (p1) {
			//l1 = strlen(p1);
			//hash = icase ? strihash(p1) : strhash(p1);
			//p2 = strtok(NULL, DELIM);
			//if (p2)
				//l2 = strlen(p2);
		//}

		//if (!strcmp("hash", cmd) && l1) {

			//[> print results of different hash functions <]
			//printf("%u %u %u %u\n", strhash(p1), memhash(p1, l1),
					//strihash(p1), memihash(p1, l1));

		//} else if (!strcmp("add", cmd) && l1 && l2) {

			//[> create entry with key = p1, value = p2 <]
			//entry = alloc_test_entry(hash, p1, l1, p2, l2);

			//[> add to hashmap <]
			//hashmap_add(&map, entry);

		//} else if (!strcmp("put", cmd) && l1 && l2) {

			//[> create entry with key = p1, value = p2 <]
			//entry = alloc_test_entry(hash, p1, l1, p2, l2);

			//[> add / replace entry <]
			//entry = hashmap_put(&map, entry);

			//[> print and free replaced entry, if any <]
			//puts(entry ? get_value(entry) : "NULL");
			//free(entry);

		//} else if (!strcmp("get", cmd) && l1) {

			//[> lookup entry in hashmap <]
			//entry = hashmap_get_from_hash(&map, hash, p1);

			//[> print result <]
			//if (!entry)
				//puts("NULL");
			//while (entry) {
				//puts(get_value(entry));
				//entry = hashmap_get_next(&map, entry);
			//}

		//} else if (!strcmp("remove", cmd) && l1) {

			//[> setup static key <]
			//struct hashmap_entry key;
			//hashmap_entry_init(&key, hash);

			//[> remove entry from hashmap <]
			//entry = hashmap_remove(&map, &key, p1);

			//[> print result and free entry<]
			//puts(entry ? get_value(entry) : "NULL");
			//free(entry);

		//} else if (!strcmp("iterate", cmd)) {

			//struct hashmap_iter iter;
			//hashmap_iter_init(&map, &iter);
			//while ((entry = hashmap_iter_next(&iter)))
				//printf("%s %s\n", entry->key, get_value(entry));

		//} else if (!strcmp("size", cmd)) {

			//[> print table sizes <]
			//printf("%u %u\n", map.tablesize, map.size);

		//} else if (!strcmp("intern", cmd) && l1) {

			//[> test that strintern works <]
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

		//} else if (!strcmp("perfhashmap", cmd) && l1 && l2) {

			//perf_hashmap(atoi(p1), atoi(p2));

		//} else {

			//printf("Unknown command %s\n", cmd);

		//}
	//}

	//hashmap_free(&map, 1);
	//return 0;
//}

		void linkedHashMap_test() {
			suite("LinkedHashMap");
			mytest(read_index);
		}

	}
}

