#ifndef SRC_CACHE_STRING_LIST_H
#define SRC_CACHE_STRING_LIST_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include "utils/console_colors.h"

namespace cache {
	
	struct string_list_item {
		char *string;
		void *util;

		bool isNotEmpty() {
			return *string != '\0';
		}
	};



#define REALLOC_ARRAY(x, alloc) (x) = xrealloc((x), st_mult(sizeof(*(x)), (alloc)))

#define alloc_nr(x) (((x)+16)*3/2)

/*
 * Realloc the buffer pointed at by variable 'x' so that it can hold
 * at least 'nr' entries; the number of entries currently allocated
 * is 'alloc', using the standard growing factor alloc_nr() macro.
 *
 * DO NOT USE any expression with side-effect for 'x', 'nr', or 'alloc'.
 */
#define ALLOC_GROW(x, nr, alloc) \
	do { \
		if ((nr) > alloc) { \
			if (alloc_nr(alloc) < (nr)) \
				alloc = (nr); \
			else \
				alloc = alloc_nr(alloc); \
			REALLOC_ARRAY(x, alloc); \
		} \
	} while (0)

#define QSORT_S(base, n, compar, ctx) do {			\
	if (qsort_s((base), (n), sizeof(*(base)), compar, ctx))	\
		die("BUG: qsort_s() failed");			\
} while (0)

#ifndef HAVE_ISO_QSORT_S
int git_qsort_s(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *), void *ctx);
#define qsort_s git_qsort_s
#endif

	/*
	 */
	class StingList {

		typedef int (*compare_strings_fn)(const char *, const char *);

		public:

			StringList(compare_strings_fn _cmp, bool _strdup_strings=true) :
			items(nullptr), nr(0), alloc(0), strdup_strings(_strdup_strings) {
				if (_cmp == nullptr) {
					cmp = std::strcmp;
				} else {
					cmp = _cmp;
				}
			}
			
			/* if there is no exact match, point to the index where the entry could be
			 * inserted */
			int getEntryIndex(const char *string, int *exact_match) {
				int left = -1, right = nr;

				while (left + 1 < right) {
					int middle = (left + right) / 2;
					int compare = cmp(string, items[middle].string);
					if (compare < 0) {
						right = middle;
					} else if (compare > 0) {
						left = middle;
					} else {
						*exact_match = 1;
						return middle;
					}
				}

				*exact_match = 0;
				return right;
			}

			/* returns -1-index if already exists */
			int addEntry(int insert_at, const char *string) {
				int exact_match = 0;
				unsigned int index = insert_at >= 0 ? insert_at : getEntryIndex(string, &exact_match);

				if (exact_match) {
					return -1 - index;
				}

				if ((nr + 1) >= alloc) {
					alloc += 32;
					reallocArray();
				}
				if (index < nr) {
					std::memmove(items + index + 1, items + index, (nr - index) * sizeof(string_list_item));
				}
				items[index].string = strdup_strings ? strdup(string) : (char *)string;
				items[index].util = nullptr;
				nr++;

				return index;
			}

			string_list_item *insert(const char *string) {
				int index = addEntry(-1, string);

				if (index < 0) {
					index = -1 - index;
				}

				return items + index;
			}

			int hasString(const char *string) {
				int exact_match;
				getEntryIndex(string, &exact_match);
				return exact_match;
			}

			int findInsertIndex(const char *string, bool negative_existing_index) {
				int exact_match;
				int index = getEntryIndex(string, &exact_match);
				if (exact_match) {
					index = -1 - (negative_existing_index ? index : 0);
				}
				return index;
			}

			string_list_item *lookup(const char *string) {
				int exact_match, i = getEntryIndex(string, &exact_match);
				if (!exact_match) {
					return nullptr;
				}
				return items + i;
			}

			void removeDuplicates(bool free_util) {
				if (nr > 1) {
					unsigned int src, dst;
					//compare_strings_fn cmp = list->cmp ? list->cmp : strcmp;
					for (src = dst = 1; src < nr; src++) {
						if (!cmp(items[dst - 1].string, items[src].string)) {
							if (strdup_strings) {
								std::free(items[src].string);
							}
							if (free_util) {
								std::free(items[src].util);
							}
						} else {
							items[dst++] = items[src];
						}
					}
					nr = dst;
				}
			}

			// iterator implementation for use in C++11 range-based for loops
			class Iterator {
				public:
					Iterator(string_list_item *_arr, unsigned int _pos) :
						arr(_arr), pos(_pos) {
						}

					bool operator!=(const Iterator &other) const {
						return pos != other.pos;
					}

					string_list_item &operator*() const {
						return arr[pos];
					}

					const Iterator &operator++() {
						++pos;
						return *this;
					}

				private:
					unsigned int pos;
					string_list_item *arr;
			};

			// begin method range-based for loop
			Iterator begin() const {
				return Iterator(items, 0);
			}

			// end method range-based for loop
			Iterator end() const {
				return Iterator(items, nr);
			}
			/////////////////////////////////


			/* Use this function or the macro below to iterate over each item */
			typedef int (*string_list_each_func_t)(struct string_list_item *, void *);
			int forEach(string_list_each_func_t fn, void *cb_data) {
				int ret = 0;
				for (unsigned int i = 0; i < nr; i++)
					if ((ret = fn(&items[i], cb_data)))
						break;
				return ret;
			}

			void filter(bool free_util, string_list_each_func_t want, void *cb_data) {
				unsigned int src, dst = 0;
				for (src = 0; src < nr; src++) {
					if (want(&items[src], cb_data)) {
						items[dst++] = items[src];
					} else {
						if (strdup_strings)
							std::free(items[src].string);
						if (free_util)
							std::free(items[src].util);
					}
				}
				nr = dst;
			}

			void removeEmptyItems(bool free_util) {
				filter(free_util, item_is_not_empty, nullptr);
			}

			void clear(bool free_util) {
				if (items) {
					if (strdup_strings) {
						for (unsigned int i = 0; i < nr; i++) {
							std::free(items[i].string);
						}
					}
					if (free_util) {
						for (unsigned int i = 0; i < nr; i++) {
							std::free(items[i].util);
						}
					}
					free(items);
				}
				items = nullptr;
				nr = alloc = 0;
			}

			string_list_item *appendNodup(char *string) {
				struct string_list_item *retval;
				allocGrow(nr + 1);
				retval = &items[nr++];
				retval->string = string;
				retval->util = nullptr;
				return retval;
			}

			string_list_item *append(const char *string) {
				return appendNodup(strdup_strings ? strdup(string) : (char *)string);
			}

			//static int cmp_items(const void *a, const void *b, void *ctx)
			//{
				//compare_strings_fn cmp = ctx;
				//const struct string_list_item *one = a;
				//const struct string_list_item *two = b;
				//return cmp(one->string, two->string);
			//}

			void sort() {
				QSORT_S(items, nr, cmp_items, cmp);
			}
			
		private:

			string_list_item *items;
			unsigned int nr, alloc;
			bool strdup_strings;
			compare_strings_fn cmp; /* NULL uses strcmp() */
			
			void reallocArray() {
				size_t amount = sizeof(string_list_item) * alloc;
				string_list_item *newItems = (string_list_item *) std::realloc(items, amount);

				if (newItems == nullptr) {
					throw std::runtime_error(std::strerror(errno));
				}
				items = newItems;
			}

			void allocGrow(unsigned int newSize) {
				if (newSize > alloc) {
					unsigned int newAlloc = (alloc+16)*3/2;
					if (newAlloc < newSize) { 
						alloc = newSize; 
					} else {
						alloc = newAlloc;
					}
					reallocArray();
				} 
			}

			static int item_is_not_empty(string_list_item *item, void *unused) {
				return *item->string != '\0';
			}
			
	};

}

#endif
