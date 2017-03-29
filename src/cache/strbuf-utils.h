#ifndef SRC_CACHE_STRBUF_UTILS_H
#define SRC_CACHE_STRBUF_UTILS_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include "cache/StringBuffer.h"

namespace cache {

	int is_rfc3986_reserved(char ch); 
	int is_rfc3986_unreserved(char ch);

	bool starts_with(const char *str, const char *prefix); 

	void addUniqueAbbrev(const StringBuffer *a, const unsigned char *sha1, int abbrev_len);

	int strbuf_cmp(const StringBuffer *a, const StringBuffer *b) {
		int len = a->len < b->len ? a->len: b->len;
		int cmp = std::memcmp(a->buf, b->buf, len);
		if (cmp) {
			return cmp;
		}
		return a->len < b->len ? -1: a->len != b->len;
	}
	
	void strbuf_add_unique_abbrev(struct strbuf *sb, const unsigned char *sha1,
			int abbrev_len)
	{
		int r;
		strbuf_grow(sb, GIT_SHA1_HEXSZ + 1);
		r = find_unique_abbrev_r(sb->buf + sb->len, sha1, abbrev_len);
		strbuf_setlen(sb, sb->len + r);
	}

	class StringSplitter {
		public:
			StringSplitter() : array(nullptr), nr(0), alloc(0) {
			}
			
			~StringSplitter() {
				release();	
			}
			
			void release() {
				if(array) {
					StringBuffer **s = array;

					while (*s) {
						delete *s;
						*s = nullptr;
						s++;// TODO: diffrent from original src
					}
					std::free(array);
					array = nullptr;
					alloc = 0;
					nr = 0;
				}
			}

			/*
			 * create array of pointer on StringBuffer.
			 * Last array element is nullptr.
			 */
			StringBuffer **splitBuf(const char *str, size_t slen,
					int terminator, size_t max) {
				while (slen) {
					int len = slen;
					if (max <= 0 || nr + 1 < max) {
						const char *end =(const char*) memchr(str, terminator, slen);
						if (end) {
							len = end - str + 1;
						}
					}
					StringBuffer *t = new StringBuffer(len);
					t->add(str, len);
					grow(nr + 2);
					array[nr++] = t;
					str += len;
					slen -= len;
				}
				grow(nr + 1); /* In case string was empty */
				array[nr] = nullptr;
				return array;
			}

		private:
			StringBuffer **array;
			size_t nr, alloc;

			void grow(size_t newLen) {
				if (newLen > alloc) {
					size_t newAlloc = (alloc+16)*3/2;
					if (newAlloc < newLen) { 
						alloc = newLen; 
					} else {
						alloc = newAlloc;
					}
					reallocArray();
				}
			}

			void reallocArray() {
				size_t amount = sizeof(StringBuffer*) * alloc;
				StringBuffer **newArray = (StringBuffer **) std::realloc(array, amount);

				if (newArray == nullptr) {
					throw std::runtime_error(std::strerror(errno));
				}
				array = newArray;
			}

	};
		

	
}

#endif
