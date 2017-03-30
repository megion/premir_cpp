#include "cache/sha1/sha1-name.h"

namespace cache {
	namespace sha1 {

		static void rearrange_packed_git(void)
		{
			packed_git = llist_mergesort(packed_git, get_next_packed_git,
					set_next_packed_git, sort_pack);
		}

		static void prepare_packed_git_mru(void)
		{
			struct packed_git *p;

			mru_clear(packed_git_mru);
			for (p = packed_git; p; p = p->next)
				mru_append(packed_git_mru, p);
		}

		static int prepare_packed_git_run_once = 0;
		void prepare_packed_git(void)
		{
			struct alternate_object_database *alt;

			if (prepare_packed_git_run_once)
				return;
			prepare_packed_git_one(get_object_directory(), 1);
			prepare_alt_odb();
			for (alt = alt_odb_list; alt; alt = alt->next)
				prepare_packed_git_one(alt->path, 0);
			rearrange_packed_git();
			prepare_packed_git_mru();
			prepare_packed_git_run_once = 1;
		}

		static int approximate_object_count_valid;

		/*
		 * Give a fast, rough count of the number of objects in the repository. This
		 * ignores loose objects completely. If you have a lot of them, then either
		 * you should repack because your performance will be awful, or they are
		 * all unreachable objects about to be pruned, in which case they're not really
		 * interesting as a measure of repo size in the first place.
		 */
		unsigned long approximate_object_count(void)
		{
			static unsigned long count;
			if (!approximate_object_count_valid) {
				struct packed_git *p;

				prepare_packed_git();
				count = 0;
				for (p = packed_git; p; p = p->next) {
					if (open_pack_index(p))
						continue;
					count += p->num_objects;
				}
			}
			return count;
		}

	}
}


