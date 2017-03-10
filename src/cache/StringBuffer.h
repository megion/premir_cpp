#ifndef SRC_CACHE_STRBUF_H
#define SRC_CACHE_STRBUF_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>
#include "cache/encoding/utf8.h"
#include "cache/wrapper.h"

namespace cache {

	bool starts_with(const char *str, const char *prefix);



	class StringBuffer {
		public:
			StringBuffer(): alloc(0), len(0), buf(nullptr) {
			}

			StringBuffer(size_t hint): alloc(0), len(0), buf(nullptr) {
				grow(hint);
			}
			
			//strbuf_split_buf
			StringBuffer(const char *str, size_t slen, int terminator, size_t max) {
				size_t nr = 0, alloc = 0;
				struct strbuf *t;

				while (slen) {
					int len = slen;
					if (max <= 0 || nr + 1 < max) {
						const char *end = memchr(str, terminator, slen);
						if (end)
							len = end - str + 1;
					}
					t = xmalloc(sizeof(struct strbuf));
					strbuf_init(t, len);
					strbuf_add(t, str, len);
					ALLOC_GROW(ret, nr + 2, alloc);
					ret[nr++] = t;
					str += len;
					slen -= len;
				}
				ALLOC_GROW(ret, nr + 1, alloc); /* In case string was empty */
				ret[nr] = NULL;

			}
			struct strbuf **strbuf_split_buf(const char *str, size_t slen,
					int terminator, int max)
			{
				struct strbuf **ret = NULL;
				size_t nr = 0, alloc = 0;
				struct strbuf *t;

				while (slen) {
					int len = slen;
					if (max <= 0 || nr + 1 < max) {
						const char *end = memchr(str, terminator, slen);
						if (end)
							len = end - str + 1;
					}
					t = xmalloc(sizeof(struct strbuf));
					strbuf_init(t, len);
					strbuf_add(t, str, len);
					ALLOC_GROW(ret, nr + 2, alloc);
					ret[nr++] = t;
					str += len;
					slen -= len;
				}
				ALLOC_GROW(ret, nr + 1, alloc); /* In case string was empty */
				ret[nr] = NULL;
				return ret;
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

			int reencode(const char *from, const char *to) {
				if (encoding::same_encoding(from, to)) {
					return 0;
				}

				int inlen;
				char *out = encoding::reencode_string_len(buf, len, to, from, &inlen);
				if (!out) {
					return -1;
				}

				attach(out, inlen, inlen);
				return 0;
			}
			
			typedef enum {
				ALIGN_LEFT,
				ALIGN_MIDDLE,
				ALIGN_RIGHT
			} align_type;

			/*
			 * Align the string given and store it into a strbuf as per the
			 * 'position' and 'width'. If the given string length is larger than
			 * 'width' than then the input string is not truncated and no
			 * alignment is done.
			 */
			void utf8Align(align_type position, unsigned int width, const char *s) {
				int slen = strlen(s);
				int display_len = encoding::utf8_strnwidth(s, slen, 0);
				int utf8_compensation = slen - display_len;

				if (display_len >= width) {
					strbuf_addstr(buf, s);
					return;
				}

				if (position == ALIGN_LEFT)
					strbuf_addf(buf, "%-*s", width + utf8_compensation, s);
				else if (position == ALIGN_MIDDLE) {
					int left = (width - display_len) / 2;
					strbuf_addf(buf, "%*s%-*s", left, "", width - left + utf8_compensation, s);
				} else if (position == ALIGN_RIGHT)
					strbuf_addf(buf, "%*s", width + utf8_compensation, s);
			}

			/*
			 * Wrap the text, if necessary. The variable indent is the indent for the
			 * first line, indent2 is the indent for all other lines.
			 * If indent is negative, assume that already -indent columns have been
			 * consumed (and no extra indent is necessary for the first line).
			 */
			void addWrappedText(const char *text, int indent1, int indent2, int width) {
				int indent, w, assume_utf8 = 1;
				const char *bol, *space, *start = text;
				size_t orig_len = len;

				if (width <= 0) {
					add_indented_text(text, indent1, indent2);
					return;
				}

retry:
				bol = text;
				w = indent = indent1;
				space = NULL;
				if (indent < 0) {
					w = -indent;
					space = text;
				}

				for (;;) {
					char c;
					size_t skip;

					while ((skip = encoding::display_mode_esc_sequence_len(text))) {
						text += skip;
					}

					c = *text;
					if (!c || isspace(c)) {
						if (w <= width || !space) {
							const char *start = bol;
							if (!c && text == start) {
								return;
							}
							if (space) {
								start = space;
							} else {
								strbuf_addchars(buf, ' ', indent);
							}
							strbuf_add(buf, start, text - start);
							if (!c) {
								return;
							}
							space = text;
							if (c == '\t') {
								w |= 0x07;
							} else if (c == '\n') {
								space++;
								if (*space == '\n') {
									strbuf_addch(buf, '\n');
									goto new_line;
								} else if (!isalnum(*space)) {
									goto new_line;
								} else {
									strbuf_addch(buf, ' ');
								}
							}
							w++;
							text++;
						}
						else {
new_line:
							strbuf_addch(buf, '\n');
							text = bol = space + isspace(*space);
							space = NULL;
							w = indent = indent2;
						}
						continue;
					}
					if (assume_utf8) {
						w += utf8_width(&text, NULL);
						if (!text) {
							assume_utf8 = 0;
							text = start;
							strbuf_setlen(buf, orig_len);
							goto retry;
						}
					} else {
						w++;
						text++;
					}
				}
			}

			void addWrappedBytes(const char *data, int len, int indent, int indent2, int width) {
				char *tmp = xstrndup(data, len);
				addWrappedText(tmp, indent, indent2, width);
				free(tmp);
			}

			void utf8Replace(int pos, int width, const char *subst) {
				StringBuffer sb_dst;
				char *src = buf;
				char *end = src + len;
				int w = 0, subst_len = 0;

				if (subst) {
					subst_len = strlen(subst);
				}
				sb_dst.grow(len + subst_len);
				char *dst = sb_dst.buf;

				while (src < end) {
					char *old;
					size_t n;

					while ((n = encoding::display_mode_esc_sequence_len(src))) {
						std::memcpy(dst, src, n);
						src += n;
						dst += n;
					}

					if (src >= end)
						break;

					old = src;
					n = utf8_width((const char**)&src, NULL);
					if (!src) 	/* broken utf-8, do nothing */
						return;
					if (n && w >= pos && w < pos + width) {
						if (subst) {
							memcpy(dst, subst, subst_len);
							dst += subst_len;
							subst = NULL;
						}
						w += n;
						continue;
					}
					memcpy(dst, old, src - old);
					dst += src - old;
					w += n;
				}
				strbuf_setlen(&sb_dst, dst - sb_dst.buf);
				strbuf_swap(sb_src, &sb_dst);
				//strbuf_release(&sb_dst);
			}

			void tolower() {
				char *p = buf, *end = buf + len;
				for (; p < end; p++) {
					*p = std::tolower(*p);
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

			void add_indented_text(const char *text, int indent, int indent2) {
				if (indent < 0) {
					indent = 0;
				}
				while (*text) {
					const char *eol = strchrnul(text, '\n');
					if (*eol == '\n') {
						eol++;
					}
					strbuf_addchars(buf, ' ', indent);
					strbuf_add(buf, text, eol - text);
					text = eol;
					indent = indent2;
				}
			}

	};

}

#endif
