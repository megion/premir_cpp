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

	/*
	 * The character that begins a commented line in user-editable file
	 * that is subject to stripspace.
	 */
	char comment_line_char = '#';
	
	class StringBuffer {

		/**
		 * Used as callback for `strbuf_expand()`, expects an array of
		 * struct strbuf_expand_dict_entry as context, i.e. pairs of
		 * placeholder and replacement string.  The array needs to be
		 * terminated by an entry with placeholder set to NULL.
		 */
		struct strbuf_expand_dict_entry {
			const char *placeholder;
			const char *value;
		};

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
			
			/*
			 * cut 'removeLen' elements start with 'pos' and insert data
			 */
			void splice(size_t pos, size_t removeLen, const void *data, size_t dataLen) {
				if (pos > len) {
					LOG(ERR, "`pos' is too far after the end of the buffer");
					return;
				}
				if (pos + removeLen > len) {
					LOG(ERR, "`pos + removeLen' is too far after the end of the buffer");
					return;
				}

				if (dataLen >= removeLen) {
					grow(dataLen - removeLen);
				}
				std::memmove(buf + pos + dataLen, buf + pos + removeLen,
						len - pos - removeLen);
				std::memcpy(buf + pos, data, dataLen);
				setLen(len + dataLen - removeLen);
			}

			void insert(size_t pos, const void *data, size_t dataLen) {
				splice(pos, 0, data, dataLen);
			}

			void remove(size_t pos, size_t removeLen) {
				splice(pos, removeLen, "", 0);
			}
			
			/**
			 * add data of given length to the buffer.
			 */
			void add(const void *data, size_t dataLen) {
				grow(dataLen);
				memcpy(buf + len, data, dataLen);
				setLen(len + dataLen);
			}

			/**
			 * Add a NUL-terminated string to the buffer.
			 *
			 * NOTE: This function will *always* be implemented as an inline or a macro
			 * using strlen, meaning that this is efficient to write things like:
			 *
			 *     strbuf_addstr(sb, "immediate string");
			 *
			 */
			void addstr(const char *s) {
				add(s, std::strlen(s));
			}

			/**
			 * Copy the contents of another buffer at the end of the current one.
			 */
			void addbuf(const StringBuffer *sb2) {
				grow(sb2->len);
				memcpy(buf + len, sb2->buf, sb2->len);
				setLen(len + sb2->len);
			}

			/**
			 * Copy part of the buffer from a given position till a given length to the
			 * end of the buffer.
			 */
			void adddup(size_t pos, size_t dupLen) {
				grow(dupLen);
				memcpy(buf + len, buf + pos, dupLen);
				setLen(len + dupLen);
			}

			void addchars(int c, size_t num) {
				grow(num);
				std::memset(buf + len, c, num);
				setLen(len + num);
			}

			void addf(const char *fmt, ...) {
				va_list ap;
				va_start(ap, fmt);
				strbuf_vaddf(sb, fmt, ap);
				va_end(ap);
			}

			void addLines(const char *prefix1, const char *prefix2,
					const char *bufLines, size_t size) {
				while (size) {
					const char *prefix;
					const char *next = memchr(bufLines, '\n', size);
					next = next ? (next + 1) : (bufLines + size);

					prefix = ((prefix2 && (bufLines[0] == '\n' || bufLines[0] == '\t'))
							? prefix2 : prefix1);
					strbuf_addstr(out, prefix);
					add(bufLines, next - bufLines);
					size -= next - buf;
					buf = next;
				}
				strbuf_complete_line(out);
			}

			/**
			 * Add a NUL-terminated string to the buffer. Each line will be prepended
			 * by a comment character and a blank.
			 */
			void addCommentedLines(const char *bufLines, size_t size) {
				static char prefix1[3];
				static char prefix2[2];

				if (prefix1[0] != comment_line_char) {
					xsnprintf(prefix1, sizeof(prefix1), "%c ", comment_line_char);
					xsnprintf(prefix2, sizeof(prefix2), "%c", comment_line_char);
				}
				addLines(prefix1, prefix2, bufLines, size);
			}

			void commentedAddf(const char *fmt, ...) {
				va_list params;
				StringBuffer tempBuf;
				int incomplete_line = len && buf[len - 1] != '\n';

				va_start(params, fmt);
				tempBuf.vaddf(fmt, params);
				va_end(params);

				addCommentedLines(tempBuf.buf, tempBuf.len);
				if (incomplete_line) {
					buf[--len] = '\0';
				}
			}

			void vaddf(const char *fmt, va_list ap) {
				int addLen;
				va_list cp;

				if (!strbuf_avail(sb)) {
					grow(64);
				}
				va_copy(cp, ap);
				addLen = vsnprintf(buf + len, alloc - len, fmt, cp);
				va_end(cp);
				if (addLen < 0) {
					LOG(ERR, "BUG: your vsnprintf is broken (returned %d)", len);
					return;
				}
				if (addLen > strbuf_avail(sb)) {
					grow(addLen);
					addLen = vsnprintf(buf + len, alloc - len, fmt, ap);
					if (addLen > strbuf_avail(sb)) {
						LOG(ERR, "BUG: your vsnprintf is broken (insatiable)");
					}
				}
				setLen(len + addLen);
			}

			/**
			 * This function can be used to expand a format string containing
			 * placeholders. To that end, it parses the string and calls the specified
			 * function for every percent sign found.
			 *
			 * The callback function is given a pointer to the character after the `%`
			 * and a pointer to the struct strbuf.  It is expected to add the expanded
			 * version of the placeholder to the strbuf, e.g. to add a newline
			 * character if the letter `n` appears after a `%`.  The function returns
			 * the length of the placeholder recognized and `strbuf_expand()` skips
			 * over it.
			 *
			 * The format `%%` is automatically expanded to a single `%` as a quoting
			 * mechanism; callers do not need to handle the `%` placeholder themselves,
			 * and the callback function will not be invoked for this placeholder.
			 *
			 * All other characters (non-percent and not skipped ones) are copied
			 * verbatim to the strbuf.  If the callback returned zero, meaning that the
			 * placeholder is unknown, then the percent sign is copied, too.
			 *
			 * In order to facilitate caching and to make it possible to give
			 * parameters to the callback, `strbuf_expand()` passes a context pointer,
			 * which can be used by the programmer of the callback as she sees fit.
			 */
			typedef size_t (*expand_fn_t) (StringBuffer *sb, const char *placeholder,
				   	void *context);
			void expand(const char *format, expand_fn_t fn, void *context) {
				for (;;) {
					const char *percent;
					size_t consumed;

					percent = strchrnul(format, '%');
					add(format, percent - format);
					if (!*percent) {
						break;
					}
					format = percent + 1;

					if (*format == '%') {
						addch('%');
						format++;
						continue;
					}

					consumed = fn(sb, format, context);
					if (consumed) {
						format += consumed;
					} else {
						addch('%');
					}
				}
			}

			size_t expand_dict_cb(const char *placeholder, void *context) {
				strbuf_expand_dict_entry *e = context;
				size_t len;

				for (; e->placeholder && (len = std::strlen(e->placeholder)); e++) {
					if (!strncmp(placeholder, e->placeholder, len)) {
						if (e->value) {
							addstr(e->value);
						}
						return len;
					}
				}
				return 0;
			}

			size_t fileRead(size_t size, FILE *f) {
				size_t oldalloc = alloc;

				grow(size);
				size_t res = fread(buf + len, 1, size, f);
				if (res > 0) {
					setLen(len + res);
				} else if (oldalloc == 0) {
					release();
				}
				return res;
			}

ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
	size_t oldlen = sb->len;
	size_t oldalloc = sb->alloc;

	strbuf_grow(sb, hint ? hint : 8192);
	for (;;) {
		ssize_t want = sb->alloc - sb->len - 1;
		ssize_t got = read_in_full(fd, sb->buf + sb->len, want);

		if (got < 0) {
			if (oldalloc == 0)
				strbuf_release(sb);
			else
				strbuf_setlen(sb, oldlen);
			return -1;
		}
		sb->len += got;
		if (got < want)
			break;
		strbuf_grow(sb, 8192);
	}

	sb->buf[sb->len] = '\0';
	return sb->len - oldlen;
}

			void addbufRercentquote(struct strbuf *dst) {
				for (size_t i = 0; i < len; i++) {
					if (buf[i] == '%') {
						dst->addch('%');
					}
					dst->addch(buf[i]);
				}
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
					addchars(' ', indent);
					add(text, eol - text);
					text = eol;
					indent = indent2;
				}
			}

		private:
			size_t alloc;

			void reallocBuf() {
				size_t amount = sizeof(char) * alloc;
				char *newBuf = (char *) std::realloc(buf, amount);

				if (newBuf == nullptr) {
					throw std::runtime_error(std::strerror(errno));
				}
				buf = newBuf;
			}

		public:
			size_t len;
			char *buf;
	};

}

#endif
