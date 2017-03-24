#ifndef SRC_CACHE_STRBUF_H
#define SRC_CACHE_STRBUF_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <limits.h>

#include <exception>
#include <stdexcept>
#include <iostream>
#include <fcntl.h>
#include "utils/console_colors.h"
#include "cache/encoding/utf8.h"
#include "cache/wrapper.h"


#define STRBUF_MAXLINK (2*PATH_MAX)

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
		
			/**
			 * Empty the buffer by setting the size of it to zero.
			 * strbuf_reset(sb)  strbuf_setlen(sb, 0)
			 */
			void reset() {	
				setLen(0);
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
			 * Determine the amount of allocated but unused memory.
			 */
			size_t avail() const {
				return alloc ? alloc - len - 1 : 0;
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

			/**
			 * Add a formatted string to the buffer.
			 */
			void addf(const char *fmt, ...) {
				va_list ap;
				va_start(ap, fmt);
				vaddf(fmt, ap);
				va_end(ap);
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
				add_lines(prefix1, prefix2, bufLines, size);
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
				va_list cp;

				if (!avail()) {
					grow(64);
				}
				va_copy(cp, ap);
				int addLen = vsnprintf(buf + len, alloc - len, fmt, cp);
				va_end(cp);
				if (addLen < 0) {
					LOG(ERR, "BUG: your vsnprintf is broken (returned %d)", addLen);
					return;
				}
				if (addLen > avail()) {
					grow(addLen);
					addLen = vsnprintf(buf + len, alloc - len, fmt, ap);
					if (addLen > avail()) {
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

			size_t strbuf_fread(size_t size, FILE *f) {
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

			ssize_t readFile(int fd, size_t hint) {
				size_t oldlen = len;
				size_t oldalloc = alloc;

				grow(hint ? hint : 8192);
				for (;;) {
					ssize_t want = alloc - len - 1;
					ssize_t got = read_in_full(fd, buf + len, want);

					if (got < 0) {
						if (oldalloc == 0) {
							release();
						} else {
							setLen(oldlen);
						}
						return -1;
					}
					len += got;
					if (got < want) {
						break;
					}
					grow(8192);
				}

				buf[len] = '\0';
				return len - oldlen;
			}

			ssize_t readFileByPath(const char *path, size_t hint) {
				int fd = open(path, O_RDONLY);
				if (fd < 0) {
					return -1;
				}
				ssize_t rlen = readFile(fd, hint);
				close(fd);
				if (rlen < 0) {
					return -1;
				}

				return rlen;
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

			ssize_t readOnce(int fd, size_t hint) {
				grow(hint ? hint : 8192);
				ssize_t cnt = xread(fd, buf + len, alloc - len - 1);
				if (cnt > 0) {
					setLen(len + cnt);
				}
				return cnt;
			}

			/*
			 * strbuf_write
			 */
			ssize_t strbuf_write(FILE *f) {
				return len ? fwrite(buf, 1, len, f) : 0;
			}

			/*
			 * strbuf_readlink
			 */
			int readLink(const char *path, size_t hint) {
				size_t oldalloc = alloc;

				if (hint < 32) {
					hint = 32;
				}

				while (hint < STRBUF_MAXLINK) {
					grow(hint);
					int newLen = readlink(path, buf, hint);
					if (newLen < 0) {
						if (errno != ERANGE) {
							break;
						}
					} else if (newLen < hint) {
						setLen(newLen);
						return 0;
					}

					/* .. the buffer was too small - try again */
					hint *= 2;
				}
				if (oldalloc == 0) {
					release();
				}
				return -1;
			}

			int getCwd() {
				size_t oldalloc = alloc;
				size_t guessed_len = 128;

				for (;; guessed_len *= 2) {
					grow(guessed_len);
					if (getcwd(buf, alloc)) {
						setLen(std::strlen(buf));
						return 0;
					}
					if (errno != ERANGE) {
						break;
					}
				}
				if (oldalloc == 0) {
					release();
				} else {
					reset();
				}
				return -1;
			}

			/*
			 * Define HAVE_GETDELIM if your system has the getdelim() function.
			 */
#ifdef HAVE_GETDELIM
			int getWholeLine(FILE *fp, int term) {
				ssize_t r;

				if (feof(fp)) {
					return EOF;
				}

				reset();

				/* Translate slopbuf to NULL, as we cannot call realloc on it */
				if (!alloc) {
					buf = nullptr;
				}
				r = getdelim(&buf, &alloc, term, fp);

				if (r > 0) {
					len = r;
					return 0;
				}
				assert(r == -1);

				/*
				 * Normally we would have called xrealloc, which will try to free
				 * memory and recover. But we have no way to tell getdelim() to do so.
				 * Worse, we cannot try to recover ENOMEM ourselves, because we have
				 * no idea how many bytes were read by getdelim.
				 *
				 * Dying here is reasonable. It mirrors what xrealloc would do on
				 * catastrophic memory failure. We skip the opportunity to free pack
				 * memory and retry, but that's unlikely to help for a malloc small
				 * enough to hold a single line of input, anyway.
				 */
				if (errno == ENOMEM) {
					LOG(ERR, "Out of memory, getdelim failed");
					return -1;
				}

				/*
				 * Restore strbuf invariants; if getdelim left us with a NULL pointer,
				 * we can just re-init, but otherwise we should make sure that our
				 * length is empty, and that the result is NUL-terminated.
				 */
				if (!buf) {
					buf = nullptr;
					alloc = len = 0;
				} else {
					reset();
				}
				return EOF;
			}
#else
			int getWholeLine(FILE *fp, int term) {
				int ch;

				if (feof(fp)) {
					return EOF;
				}

				reset();
				flockfile(fp);
				while ((ch = getc_unlocked(fp)) != EOF) {
					if (!avail()) {
						grow(1);
					}
					buf[len++] = ch;
					if (ch == term) {
						break;
					}
				}
				funlockfile(fp);
				if (ch == EOF && len == 0) {
					return EOF;
				}

				buf[len] = '\0';
				return 0;
			}
#endif

			int strbuf_getdelim(FILE *fp, int term) {
				if (getWholeLine(fp, term)) {
					return EOF;
				}
				if (buf[len - 1] == term) {
					setLen(len - 1);
				}
				return 0;
			}

			int getLine(FILE *fp) {
				if (getWholeLine(fp, '\n')) {
					return EOF;
				}
				if (buf[len - 1] == '\n') {
					setLen(len - 1);
					if (len && buf[len - 1] == '\r') {
						setLen(len - 1);
					}
				}
				return 0;
			}

			int getLineLf(FILE *fp) {
				return strbuf_getdelim(fp, '\n');
			}

			int getLineNul(FILE *fp) {
				return strbuf_getdelim(fp, '\0');
			}

			int getWholeLineFd(int fd, int term) {
				reset();

				while (1) {
					char ch;
					ssize_t len = xread(fd, &ch, 1);
					if (len <= 0) {
						return EOF;
					}
					addch(ch);
					if (ch == term) {
						break;
					}
				}
				return 0;
			}

			void addLines(const char *prefix,
					const char *bufLines, size_t size) {
				add_lines(prefix, NULL, bufLines, size);
			}

void addstrXmlQuoted(const char *s) {
	while (*s) {
		size_t len = strcspn(s, "\"<>&");
		add(s, len);
		s += len;
		switch (*s) {
		case '"':
			strbuf_addstr(buf, "&quot;");
			break;
		case '<':
			strbuf_addstr(buf, "&lt;");
			break;
		case '>':
			strbuf_addstr(buf, "&gt;");
			break;
		case '&':
			strbuf_addstr(buf, "&amp;");
			break;
		case 0:
			return;
		}
		s++;
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

			/*
			 * add_lines
			 */
			void add_lines(const char *prefix1, 
					const char *prefix2,
					const char *bufLines,
				   	size_t size) {
				while (size) {
					const char *prefix;
					const char *next = (const char *)memchr(bufLines, '\n', size);
					next = next ? (next + 1) : (bufLines + size);

					prefix = ((prefix2 && (bufLines[0] == '\n' || bufLines[0] == '\t'))
							? prefix2 : prefix1);
					addstr(prefix);
					add(bufLines, next - bufLines);
					size -= next - bufLines;
					bufLines = next;
				}
				completeLine();
			}

		public:
			size_t len;
			char *buf;
	};

}

#endif
