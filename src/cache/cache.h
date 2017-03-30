#ifndef SRC_CACHE_CACHE_H
#define SRC_CACHE_CACHE_H

#include <stdio.h>
#include <iostream>

//#define alloc_nr(x) (((x)+16)*3/2)

//#define ALLOC_GROW(x, nr, alloc) \
	//do { \
		//if ((nr) > alloc) { \
			//if (alloc_nr(alloc) < (nr)) \
				//alloc = (nr); \
			//else \
				//alloc = alloc_nr(alloc); \
			//REALLOC_ARRAY(x, alloc); \
		//} \
	//} while (0)
	
//#define ALLOC_ARRAY(x, alloc) (x) = xmalloc(st_mult(sizeof(*(x)), (alloc)))
//#define REALLOC_ARRAY(x, alloc) (x) = xrealloc((x), st_mult(sizeof(*(x)), (alloc)))



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

namespace cache {

	/*
	 * The "cache_time" is just the low 32 bits of the
	 * time. It doesn't matter if it overflows - we only
	 * check it for equality in the 32 bits we save.
	 */
	//struct cache_time {
		//uint32_t sec;
		//uint32_t nsec;
	//};

	//struct stat_data {
		//struct cache_time sd_ctime;
		//struct cache_time sd_mtime;
		//unsigned int sd_dev;
		//unsigned int sd_ino;
		//unsigned int sd_uid;
		//unsigned int sd_gid;
		//unsigned int sd_size;
	//};

	//struct cache_entry {
		//struct hashmap_entry ent;
		//struct stat_data ce_stat_data;
		//unsigned int ce_mode;
		//unsigned int ce_flags;
		//unsigned int ce_namelen;
		//unsigned int index;	[> for link extension <]
		//struct object_id oid;
		//char name[FLEX_ARRAY]; [> more <]
	//};
	
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
