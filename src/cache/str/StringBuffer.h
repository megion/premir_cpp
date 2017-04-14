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
#include "cache/str/strbuf-utils.h"
#include "cache/compat-utils.h"


#define STRBUF_MAXLINK (2*PATH_MAX)

namespace cache {
	namespace str {

		class StringBuffer {

			/**
			 * Used as callback for `strbuf_expand()`, expects an array of
			 * struct strbuf_expand_dict_entry as context, i.e. pairs of
			 * placeholder and replacement string.  The array needs to be
			 * terminated by an entry with placeholder set to NULL.
			 */
			//struct strbuf_expand_dict_entry {
				//const char *placeholder;
				//const char *value;
			//};

			public:
			StringBuffer(): alloc(0), len(0), buf(nullptr) {
			}

			StringBuffer(size_t hint): alloc(0), len(0), buf(nullptr) {
				if (hint) {
					grow(hint);
				}
			}

			//extern char strbuf_slopbuf[];
			//#define STRBUF_INIT  { 0, 0, strbuf_slopbuf }
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

			/**
			 * Add a single character to the buffer.
			 */
			void addch(int c) {
				if (!avail()) {
					grow(1);
				}
				buf[len++] = c;
				buf[len] = '\0';
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

				if (prefix1[0] != COMMENT_LINE_CHAR) {
					xsnprintf(prefix1, sizeof(prefix1), "%c ", COMMENT_LINE_CHAR);
					xsnprintf(prefix2, sizeof(prefix2), "%c", COMMENT_LINE_CHAR);
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
							addstr("&quot;");
							break;
						case '<':
							addstr("&lt;");
							break;
						case '>':
							addstr("&gt;");
							break;
						case '&':
							addstr("&amp;");
							break;
						case 0:
							return;
					}
					s++;
				}
			}

			void addstrUrlencode(const char *s, int reserved) {
				add_urlencode(s, strlen(s), reserved);
			}

			void humaniseBytes(off_t bytes) {
				if (bytes > 1 << 30) {
					addf("%u.%2.2u GiB",
							(int)(bytes >> 30),
							(int)(bytes & ((1 << 30) - 1)) / 10737419);
				} else if (bytes > 1 << 20) {
					int x = bytes + 5243;  /* for rounding */
					addf("%u.%2.2u MiB",
							x >> 20, ((x & ((1 << 20) - 1)) * 100) >> 20);
				} else if (bytes > 1 << 10) {
					int x = bytes + 5;  /* for rounding */
					addf("%u.%2.2u KiB",
							x >> 10, ((x & ((1 << 10) - 1)) * 100) >> 10);
				} else {
					addf("%u bytes", (int)bytes);
				}
			}

			void addAbsolutePath(const char *path) {
				if (!*path) {
					LOG(ERR, "The empty string is not a valid path");
					return;
				}
				if (!is_absolute_path(path)) {
					struct stat cwd_stat, pwd_stat;
					size_t orig_len = len;
					char *cwd = xgetcwd();
					char *pwd = getenv("PWD");
					if (pwd && strcmp(pwd, cwd) &&
							!stat(cwd, &cwd_stat) &&
							(cwd_stat.st_dev || cwd_stat.st_ino) &&
							!stat(pwd, &pwd_stat) &&
							pwd_stat.st_dev == cwd_stat.st_dev &&
							pwd_stat.st_ino == cwd_stat.st_ino) {
						addstr(pwd);
					} else {
						addstr(cwd);
					}
					if (len > orig_len && !is_dir_sep(buf[len - 1])) {
						addch(sb, '/');
					}
					free(cwd);
				}
				addstr(path);
			}

			void addftime(const char *fmt, const struct tm *tm) {
				size_t hint = 128;

				if (!*fmt) {
					return;
				}

				grow(hint);
				size_t newlen = strftime(buf + len, alloc - len, fmt, tm);

				if (!newlen) {
					/*
					 * strftime reports "0" if it could not fit the result in the buffer.
					 * Unfortunately, it also reports "0" if the requested time string
					 * takes 0 bytes. So our strategy is to munge the format so that the
					 * output contains at least one character, and then drop the extra
					 * character before returning.
					 */
					StringBuffer munged_fmt;
					munged_fmt.addf("%s ", fmt);
					while (!newlen) {
						hint *= 2;
						grow(hint);
						newlen = strftime(buf + len, alloc - len,
								munged_fmt.buf, tm);
					}
					newlen--; /* drop munged space */
				}
				setLen(len + newlen);
			}

			//void addUniqueAbbrev(const unsigned char *sha1, int abbrev_len) {
			//grow(GIT_SHA1_HEXSZ + 1);
			//int r = find_unique_abbrev_r(sb->buf + sb->len, sha1, abbrev_len);
			//setLen(len + r);
			//}

			private:
			size_t alloc;

			/*
			 * The character that begins a commented line in user-editable file
			 * that is subject to stripspace.
			 */
			const static char COMMENT_LINE_CHAR = '#';

			void reallocBuf() {
				size_t amount = sizeof(char) * alloc;
				char *newBuf = (char *) std::realloc(buf, amount);

				if (newBuf == nullptr) {
					throw std::runtime_error(std::strerror(errno));
				}
				buf = newBuf;
			}

			void add_urlencode(const char *s, size_t strlen, int reserved) {
				grow(strlen);
				while (strlen--) {
					char ch = *s++;
					if (is_rfc3986_unreserved(ch) ||
							(!reserved && is_rfc3986_reserved(ch))) {
						addch(ch);
					} else {
						addf("%%%02x", ch);
					}
				}
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

			size_t getAlloc() const {
				return alloc;
			}
		};

	}
}

#endif
