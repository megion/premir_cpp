#ifndef SRC_CACHE_STRBUF_H
#define SRC_CACHE_STRBUF_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

namespace cache {

	bool starts_with(const char *str, const char *prefix);



	class StringBuffer {
		public:
			StringBuffer(): alloc(0), len(0), buf(nullptr) {
			}

			StringBuffer(size_t hint): alloc(0), len(0), buf(nullptr) {
				grow(hint);
			}

			~StringBuffer() {
				release();	
			}

			void release() {
				if(buf) {
					std::free(buf);
					alloc = 0;
					len = 0;
					buf = nullptr;
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

			int reencode(const char *from, const char *to) {
				char *out;
				int len;

				if (same_encoding(from, to))
					return 0;

				out = reencode_string_len(sb->buf, sb->len, to, from, &len);
				if (!out)
					return -1;

				strbuf_attach(sb, out, len, len);
				return 0;
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
	};

}

#endif
