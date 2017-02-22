#include "cache/str-intern.h"

namespace cache {
	
	/* initialize global string pool hashmap */
	static PoolEntryComparator interningEntryComp;
	static LinkedHashMap<pool_entry, pool_entry_data> interningMap(0, &interningEntryComp);
	
    const void *memintern(const void *data, size_t len) {

		//if (!map.tablesize)
			//hashmap_init(&map, (hashmap_cmp_fn) pool_entry_cmp, 0);

		/* lookup interned string in pool */
		pool_entry_data key;
		unsigned int hash = memhash(data, len); 
		key.len = len;
		key.data = (unsigned char*)data;
		pool_entry* e = interningMap.getEntry(hash, &key);
		if (!e) {
			/* not found: create it */
			/* allocate memory sizeof struct + data length */
			size_t amount = sizeof(pool_entry) + len;
			e = (pool_entry*)std::calloc(1, amount);
			if (e == nullptr) {
				throw std::runtime_error(std::strerror(errno));
			}
			/* copy data to the end of struct */
			std::memcpy(e->data, data, len);
			e->ent.hash = hash;
			e->ent.next = nullptr;
			e->len = len;
			interningMap.add(e);
		}
		return e->data;
	}

	const char *strintern(const char *str) {
		return (char*) memintern(str, strlen(str));
	}

}


