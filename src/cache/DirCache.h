#ifndef SRC_CACHE_DIRCACHE_H
#define SRC_CACHE_DIRCACHE_H

#include <stdio.h>
#include <iostream>

namespace cache {

	struct hashmap_entry {
		struct hashmap_entry *next;
		unsigned int hash;
	};

	/*
	 * The "cache_time" is just the low 32 bits of the
	 * time. It doesn't matter if it overflows - we only
	 * check it for equality in the 32 bits we save.
	 */
	struct cache_time {
		uint32_t sec;
		uint32_t nsec;
	};

	struct stat_data {
		struct cache_time sd_ctime;
		struct cache_time sd_mtime;
		unsigned int sd_dev;
		unsigned int sd_ino;
		unsigned int sd_uid;
		unsigned int sd_gid;
		unsigned int sd_size;
	};

	struct cache_entry {
		struct hashmap_entry ent;
		struct stat_data ce_stat_data;
		unsigned int ce_mode;
		unsigned int ce_flags;
		unsigned int ce_namelen;
		unsigned int index;	/* for link extension */
		struct object_id oid;
		char name[FLEX_ARRAY]; /* more */
	};
	
	struct index_state {
		struct cache_entry **cache;
		unsigned int version;
		unsigned int cache_nr, cache_alloc, cache_changed;
		struct string_list *resolve_undo;
		struct cache_tree *cache_tree;
		struct split_index *split_index;
		struct cache_time timestamp;
		unsigned name_hash_initialized : 1,
				 initialized : 1;
		struct hashmap name_hash;
		struct hashmap dir_hash;
		unsigned char sha1[20];
		struct untracked_cache *untracked;
	};   
}


#endif
