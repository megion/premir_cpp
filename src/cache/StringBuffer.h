#ifndef SRC_CACHE_STRBUF_H
#define SRC_CACHE_STRBUF_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>
#include "utils/console_colors.h"
#include "cache/encoding/utf8.h"
#include "cache/wrapper.h"

namespace cache {

	bool starts_with(const char *str, const char *prefix);
	
	class StringBuffer {
		public:
			StringBuffer(): alloc(0), len(0), buf(nullptr) {
			}

			StringBuffer(size_t hint): alloc(0), len(0), buf(nullptr) {
				if (hint) {
					grow(hint);
				}
			}

//void strbuf_init(struct strbuf *sb, size_t hint)
//{
	//sb->alloc = sb->len = 0;
	//sb->buf = strbuf_slopbuf;
	//if (hint)
		//strbuf_grow(sb, hint);
//}
			
			~StringBuffer() {
				release();	
			}

			char *getBuf() const {
				return buf;
			}

			size_t getLen() const {
				return len;
			}

			/**
			 * Set the length of the buffer to a given value. This function does *not*
			 * allocate new memory, so you should not perform a `strbuf_setlen()` to a
			 * length that is larger than `len + strbuf_avail()`. `strbuf_setlen()` is
			 * just meant as a 'please fix invariants from this strbuf I just messed
			 * with'.
			 */
			void setLen(size_t newLen) {
				if (newLen > (alloc ? alloc - 1 : 0)) {
					LOG(ERR, "BUG: setLen() beyond buffer");
				}
				len = newLen;
				buf[len] = '\0';
			}

			void release() {
				if(buf) {
					std::free(buf);
					alloc = 0;
					len = 0;
					buf = nullptr;
				}
			}
			
			void grow(size_t extra) {
				bool new_buf = (alloc==0);

				size_t newLen = len + extra + 1;
				if (newLen > alloc) {
					size_t newAlloc = (alloc+16)*3/2;
					if (newAlloc < newLen) { 
						alloc = newLen; 
					} else {
						alloc = newAlloc;
					}
					reallocBuf();
				}
				if (new_buf) {
					buf[0] = '\0';
				}
			}

			/**
			 * Detach the string from the strbuf and returns it; you now own the
			 * storage the string occupies and it is your responsibility from then on
			 * to release it with `free(3)` when you are done with it.
			 */
			char *detach(size_t *sz) {
				char *res;
				grow(0);
				res = buf;
				if (sz) {
					*sz = len;
				}
				alloc = 0;
				len = 0;
				buf = nullptr;
				return res;
			}

			/**
			 * Attach a string to a buffer. You should specify the string to attach,
			 * the current length of the string and the amount of allocated memory.
			 * The amount must be larger than the string length, because the string you
			 * pass is supposed to be a NUL-terminated string.  This string _must_ be
			 * malloc()ed, and after attaching, the pointer cannot be relied upon
			 * anymore, and neither be free()d directly.
			 */
			void attach(char *_buf, size_t _len, size_t _alloc) {
				release();
				buf = _buf;
				len = _len;
				alloc = _alloc;
				grow(0);
				buf[len] = '\0';
			}

			void trim() {
				rtrim();
				ltrim();
			}

			void rtrim() {
				while (len > 0 && std::isspace(buf[len - 1])) {
					len--;
				}
				buf[len] = '\0';
			}

			void ltrim() {
				char *b = buf;
				while (len > 0 && std::isspace(*b)) {
					b++;
					len--;
				}
				std::memmove(buf, b, len);
				buf[len] = '\0';
			}

			void tolower() {
				char *p = buf, *end = buf + len;
				for (; p < end; p++) {
					*p = std::tolower(*p);
				}
			}
			
			// strbuf_add
			void add(const void *data, size_t dataLen) {
				grow(dataLen);
				memcpy(buf + len, data, dataLen);
				setLen(len + dataLen);
			}

			void addIndentedText(const char *text, int indent, int indent2) {
				if (indent < 0) {
					indent = 0;
				}
				while (*text) {
					const char *eol = strchrnul(text, '\n');
					if (*eol == '\n') {
						eol++;
					}
					strbuf_addchars(buf, ' ', indent);
					add(text, eol - text);
					text = eol;
					indent = indent2;
				}
			}

		private:
			size_t alloc;
			size_t len;
			char *buf;

			void reallocBuf() {
				size_t amount = sizeof(char) * alloc;
				char *newBuf = (char *) std::realloc(buf, amount);

				if (newBuf == nullptr) {
					throw std::runtime_error(std::strerror(errno));
				}
				buf = newBuf;
			}

			

	};

}

#endif
