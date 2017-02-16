#ifndef SRC_CACHE_DIRCACHE_H
#define SRC_CACHE_DIRCACHE_H

#include <stdio.h>
#include <iostream>
#include "cache/LinkedHashMap.h"

#ifndef FLEX_ARRAY
/*
 * See if our compiler is known to support flexible array members.
 */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && (!defined(__SUNPRO_C) || (__SUNPRO_C > 0x580))
# define FLEX_ARRAY /* empty */
#elif defined(__GNUC__)
# if (__GNUC__ >= 3)
#  define FLEX_ARRAY /* empty */
# else
#  define FLEX_ARRAY 0 /* older GNU extension */
# endif
#endif

/*
 * Otherwise, default to safer but a bit wasteful traditional style
 */
#ifndef FLEX_ARRAY
# define FLEX_ARRAY 1
#endif
#endif

/* The length in bytes and in hex digits of an object name (SHA-1 value). */
#define GIT_SHA1_RAWSZ 20
#define GIT_SHA1_HEXSZ (2 * GIT_SHA1_RAWSZ)


#define read_cache() read_index(&the_index)

namespace cache {

	struct object_id {
		unsigned char hash[GIT_SHA1_RAWSZ];
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
	
	//struct index_state {
		//struct cache_entry **cache;
		//unsigned int version;
		//unsigned int cache_nr, cache_alloc, cache_changed;
		//struct string_list *resolve_undo;
		//struct cache_tree *cache_tree;
		//struct split_index *split_index;
		//struct cache_time timestamp;
		//unsigned name_hash_initialized : 1,
				 //initialized : 1;
		//struct hashmap name_hash;
		//struct hashmap dir_hash;
		//unsigned char sha1[20];
		//struct untracked_cache *untracked;
	//};   

	//int read_index(struct index_state *istate)
	//{
		//return read_index_from(istate, get_index_file());
	//}
}


#endif
