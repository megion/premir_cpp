#ifndef SRC_CACHE_ENCODING_STR_BUF_ENCODING_H
#define SRC_CACHE_ENCODING_STR_BUF_ENCODING_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include "cache/StringBuffer.h"
#include "cache/encoding/utf8.h"

namespace cache {
	namespace encoding {

		typedef enum {
			ALIGN_LEFT,
			ALIGN_MIDDLE,
			ALIGN_RIGHT
		} align_type;

		int reencode(StringBuffer *buf, const char *from, const char *to);
		void utf8Align(StringBuffer *buf, align_type position, unsigned int width, const char *s);
		void addWrappedText(StringBuffer *buf, const char *text, int indent1, int indent2, int width);
		void addWrappedBytes(StringBuffer *buf, const char *data, int len, int indent, int indent2, int width);
		void utf8Replace(StringBuffer *buf, int pos, int width, const char *subst);
	}
}

#endif
