#ifndef SRC_CACHE_STRBUF_UTILS_H
#define SRC_CACHE_STRBUF_UTILS_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include "cache/str/StringBuffer.h"

namespace cache {
	namespace str {
		int is_rfc3986_reserved(char ch); 
		int is_rfc3986_unreserved(char ch);

		bool starts_with(const char *str, const char *prefix); 
		
void addbufPercentquote(StringBuffer *dst) {
				for (size_t i = 0; i < len; i++) {
					if (buf[i] == '%') {
						dst->addch('%');
					}
					dst->addch(buf[i]);
				}
			}

		//void addUniqueAbbrev(const StringBuffer *a, const unsigned char *sha1, int abbrev_len);

		//int strbuf_cmp(const StringBuffer *a, const StringBuffer *b) {
		//int len = a->len < b->len ? a->len: b->len;
		//int cmp = std::memcmp(a->buf, b->buf, len);
		//if (cmp) {
		//return cmp;
		//}
		//return a->len < b->len ? -1: a->len != b->len;
		//}

	}
}

#endif
