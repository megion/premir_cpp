#include "cache/strintern.h"

namespace cache {

    const void *memintern(const void *data, size_t len) {
		// global static string pool 
		/* initialize string pool hashmap */
		static LinkedHashMap<pool_entry> map(0);

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

	const char *strintern(const char *string) {
		return memintern(string, strlen(string));
	}

}


