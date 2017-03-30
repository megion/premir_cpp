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
#include "cache/cache.h"
#include "cache/qsort_s.h"
#include "cache/wrapper.h"

namespace cache {
	
	template<typename T>
	struct string_list_item {
		char *string;
		T *util;
	};

	/*
	 */
	template<typename T>
	class StringList {

		public:
			
			typedef int (*compare_strings_fn)(const char *, const char *);
		
			StringList(compare_strings_fn _cmp, bool _strdup_strings = true) : 
				items(nullptr), nr(0), alloc(0), strdup_strings(_strdup_strings) {
				if (_cmp == nullptr) {
					cmp = std::strcmp;
				} else {
					cmp = _cmp;
				}
			}
			
			~StringList() {
				clear(false);
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
					std::memmove(items + index + 1, items + index, (nr - index) * sizeof(string_list_item<T>));
				}
				items[index].string = strdup_strings ? strdup(string) : (char *)string;
				items[index].util = nullptr;
				nr++;

				return index;
			}

			string_list_item<T> *insert(const char *string) {
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

			string_list_item<T> *lookup(const char *string) {
				int exact_match, i = getEntryIndex(string, &exact_match);
				if (!exact_match) {
					return nullptr;
				}
				return items + i;
			}

			/*
			 * TODO: strange logic! Only clear pointer and is not changed size list.
			 * Do not use this function.
			 */
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
			// example:
			//for (Foo2 &ff : list) {
				//ff.id = 8;
			//}
			class Iterator {
				public:
					Iterator(string_list_item<T> *_arr) : arr(_arr) {
					}

					bool operator!=(const Iterator &other) const {
						return arr!= other.arr;
					}

					string_list_item<T> &operator*() const {
						return *arr;
					}

					const Iterator &operator++() {
						++arr;
						return *this;
					}

				private:
					string_list_item<T> *arr;
			};

			// begin method range-based for loop
			Iterator begin() const {
				return Iterator(items);
			}

			// end method range-based for loop
			Iterator end() const {
				return Iterator(items + nr);
			}
			/////////////////////////////////


			/* Use this function or the macro below to iterate over each item */
			typedef bool (*string_list_each_func_t)(struct string_list_item<T> *, void *);
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
					std::free(items);
				}
				items = nullptr;
				nr = alloc = 0;
			}

			string_list_item<T> *appendNodup(char *string) {
				struct string_list_item<T> *retval;
				allocGrow(nr + 1);
				retval = &items[nr++];
				retval->string = string;
				retval->util = nullptr;
				return retval;
			}

			string_list_item<T> *append(const char *string) {
				return appendNodup(strdup_strings ? strdup(string) : (char *)string);
			}

			void sort() {
				cache::qsort_s(items, nr, cmp_items, (void*)cmp);
			}

			string_list_item<T> *unsortedLookup(const char *string) {
				for(string_list_item<T> &item: (*this)) {
					if (!cmp(string, item.string)) {
						return &item;
					}

				}
				return nullptr;
			}

			void print() {
				for(string_list_item<T> &item: (*this)) {
					std::printf("%s, ", item.string);
				}
				std::printf("\n");
			}

			bool unsortedHasString(const char *string) {
				return unsortedLookup(string) != nullptr;
			}

			void unsortedDeleteItem(int i, bool free_util) {
				if (strdup_strings) {
					std::free(items[i].string);
				}
				if (free_util) {
					std::free(items[i].util);
				}
				items[i] = items[nr-1];
				nr--;
			}

			int split(const char *string, int delim, int maxsplit) {
				int count = 0;
				const char *p = string, *end;

				if (!strdup_strings) {
					LOG(ERR, "internal error in split(): strdup_strings must be set");
				}
				for (;;) {
					count++;
					if (maxsplit >= 0 && count > maxsplit) {
						append(p);
						return count;
					}
					end = std::strchr(p, delim);
					if (end) {
						appendNodup((char*)cache::xmemdupz(p, end - p));
						p = end + 1;
					} else {
						append(p);
						return count;
					}
				}
			}

			int splitInPlace(char *string, int delim, int maxsplit) {
				int count = 0;
				char *p = string, *end;

				if (strdup_strings) {
					LOG(ERR, "internal error in splitInPlace(): strdup_strings must not be set");
				}
				for (;;) {
					count++;
					if (maxsplit >= 0 && count > maxsplit) {
						append(p);
						return count;
					}
					end = std::strchr(p, delim);
					if (end) {
						*end = '\0';
						append(p);
						p = end + 1;
					} else {
						append(p);
						return count;
					}
				}
			}
			
			unsigned int size() {
				return nr;
			}
			
			string_list_item<T> *getItems() {
				return items;
			}
			
			// [] index operator
			string_list_item<T> &operator[](const size_t &index) const {
				return items[index];
			}
			
		private:

			string_list_item<T> *items;
			unsigned int nr, alloc;
			bool strdup_strings;
			compare_strings_fn cmp; /* NULL uses strcmp() */
			
			void reallocArray() {
				size_t amount = sizeof(string_list_item<T>) * alloc;
				string_list_item<T> *newItems = (string_list_item<T> *) std::realloc(items, amount);

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

			static int item_is_not_empty(string_list_item<T> *item, void *unused) {
				return *item->string != '\0';
			}

			static int cmp_items(const void *a, const void *b, void *ctx) {
				compare_strings_fn cmpfn = (compare_strings_fn)ctx;
				const string_list_item<T> *one = (string_list_item<T>*)a;
				const string_list_item<T> *two = (string_list_item<T>*)b;
				return cmpfn(one->string, two->string);
			}
			
	};

}

#endif
