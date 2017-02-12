#ifndef SRC_CACHE_READCACHE_H
#define SRC_CACHE_READCACHE_H

#include <stdio.h>
#include <iostream>

#include "cache/DirCache.h"

namespace cache {
	
	class ReadCache {
		public:

			ReadCache() {
			}

			int read_index_from(struct index_state *istate, const char *path) {
				struct split_index *split_index;
				int ret;

				/* istate->initialized covers both .git/index and .git/sharedindex.xxx */
				if (istate->initialized)
					return istate->cache_nr;

				ret = do_read_index(istate, path, 0);

				split_index = istate->split_index;
				if (!split_index || is_null_sha1(split_index->base_sha1)) {
					post_read_index_from(istate);
					return ret;
				}

				if (split_index->base)
					discard_index(split_index->base);
				else
					split_index->base = xcalloc(1, sizeof(*split_index->base));
				ret = do_read_index(split_index->base,
						git_path("sharedindex.%s",
							sha1_to_hex(split_index->base_sha1)), 1);
				if (hashcmp(split_index->base_sha1, split_index->base->sha1))
					die("broken index, expect %s in %s, got %s",
							sha1_to_hex(split_index->base_sha1),
							git_path("sharedindex.%s",
								sha1_to_hex(split_index->base_sha1)),
							sha1_to_hex(split_index->base->sha1));
				merge_base_index(istate);
				post_read_index_from(istate);
				return ret;
			}

			void check_ce_order(struct index_state *istate) {
				unsigned int i;

				for (i = 1; i < istate->cache_nr; i++) {
					struct cache_entry *ce = istate->cache[i - 1];
					struct cache_entry *next_ce = istate->cache[i];
					int name_compare = strcmp(ce->name, next_ce->name);

					if (0 < name_compare)
						die("unordered stage entries in index");
					if (!name_compare) {
						if (!ce_stage(ce))
							die("multiple stage entries for merged file '%s'",
									ce->name);
						if (ce_stage(ce) > ce_stage(next_ce))
							die("unordered stage entries for '%s'",
									ce->name);
					}
				}
			}

			void tweak_untracked_cache(struct index_state *istate) {
				switch (git_config_get_untracked_cache()) {
					case -1: /* keep: do nothing */
						break;
					case 0: /* false */
						remove_untracked_cache(istate);
						break;
					case 1: /* true */
						add_untracked_cache(istate);
						break;
					default: /* unknown value: do nothing */
						break;
				}
			}

			void post_read_index_from(struct index_state *istate) {
				check_ce_order(istate);
				tweak_untracked_cache(istate);
			}
			
			/* remember to discard_cache() before reading a different cache! */
			int do_read_index(struct index_state *istate, const char *path, int must_exist) {
				int fd, i;
				struct stat st;
				unsigned long src_offset;
				struct cache_header *hdr;
				void *mmap;
				size_t mmap_size;
				struct strbuf previous_name_buf = STRBUF_INIT, *previous_name;

				if (istate->initialized)
					return istate->cache_nr;

				istate->timestamp.sec = 0;
				istate->timestamp.nsec = 0;
				fd = open(path, O_RDONLY);
				if (fd < 0) {
					if (!must_exist && errno == ENOENT)
						return 0;
					die_errno("%s: index file open failed", path);
				}

				if (fstat(fd, &st))
					die_errno("cannot stat the open index");

				mmap_size = xsize_t(st.st_size);
				if (mmap_size < sizeof(struct cache_header) + 20)
					die("index file smaller than expected");

				mmap = xmmap(NULL, mmap_size, PROT_READ, MAP_PRIVATE, fd, 0);
				if (mmap == MAP_FAILED)
					die_errno("unable to map index file");
				close(fd);

				hdr = mmap;
				if (verify_hdr(hdr, mmap_size) < 0)
					goto unmap;

				hashcpy(istate->sha1, (const unsigned char *)hdr + mmap_size - 20);
				istate->version = ntohl(hdr->hdr_version);
				istate->cache_nr = ntohl(hdr->hdr_entries);
				istate->cache_alloc = alloc_nr(istate->cache_nr);
				istate->cache = xcalloc(istate->cache_alloc, sizeof(*istate->cache));
				istate->initialized = 1;

				if (istate->version == 4)
					previous_name = &previous_name_buf;
				else
					previous_name = NULL;

				src_offset = sizeof(*hdr);
				for (i = 0; i < istate->cache_nr; i++) {
					struct ondisk_cache_entry *disk_ce;
					struct cache_entry *ce;
					unsigned long consumed;

					disk_ce = (struct ondisk_cache_entry *)((char *)mmap + src_offset);
					ce = create_from_disk(disk_ce, &consumed, previous_name);
					set_index_entry(istate, i, ce);

					src_offset += consumed;
				}
				strbuf_release(&previous_name_buf);
				istate->timestamp.sec = st.st_mtime;
				istate->timestamp.nsec = ST_MTIME_NSEC(st);

				while (src_offset <= mmap_size - 20 - 8) {
					/* After an array of active_nr index entries,
					 * there can be arbitrary number of extended
					 * sections, each of which is prefixed with
					 * extension name (4-byte) and section length
					 * in 4-byte network byte order.
					 */
					uint32_t extsize;
					memcpy(&extsize, (char *)mmap + src_offset + 4, 4);
					extsize = ntohl(extsize);
					if (read_index_extension(istate,
								(const char *) mmap + src_offset,
								(char *) mmap + src_offset + 8,
								extsize) < 0)
						goto unmap;
					src_offset += 8;
					src_offset += extsize;
				}
				munmap(mmap, mmap_size);
				return istate->cache_nr;

unmap:
				munmap(mmap, mmap_size);
				die("index file corrupt");
			}


	};
}


#endif
