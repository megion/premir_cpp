#include "cache/str/strbuf-utils.h"

namespace cache {
	namespace str {

		int is_rfc3986_reserved(char ch) {
			switch (ch) {
				case '!': case '*': case '\'': case '(': case ')': case ';':
				case ':': case '@': case '&': case '=': case '+': case '$':
				case ',': case '/': case '?': case '#': case '[': case ']':
					return 1;
			}
			return 0;
		}

		int is_rfc3986_unreserved(char ch) {
			return isalnum(ch) ||
				ch == '-' || ch == '_' || ch == '.' || ch == '~';
		}

		bool starts_with(const char *str, const char *prefix) {
			for (; ; str++, prefix++) {
				if (!*prefix) {
					return true;
				} else if (*str != *prefix) {
					return false;
				}
			}
		}
		
		void strbuf_addbufPercentquote(StringBuffer *dst, const StringBuffer *src) {
			for (size_t i = 0; i < src->len; i++) {
				if (src->buf[i] == '%') {
					dst->addch('%');
				}
				dst->addch(src->buf[i]);
			}
		}

		void strbuf_expand(StringBuffer *sb, const char *format, expand_fn_t fn, void *context) {
			for (;;) {
				const char *percent;
				size_t consumed;

				percent = strchrnul(format, '%');
				sb->add(format, percent - format);
				if (!*percent) {
					break;
				}
				format = percent + 1;

				if (*format == '%') {
					sb->addch('%');
					format++;
					continue;
				}

				consumed = fn(sb, format, context);
				if (consumed) {
					format += consumed;
				} else {
					sb->addch('%');
				}
			}
		}

		//void addUniqueAbbrev(StringBuffer *sb, const unsigned char *sha1, int abbrev_len) {
		//sb->grow(GIT_SHA1_HEXSZ + 1);
		//int r = find_unique_abbrev_r(sb->buf + sb->len, sha1, abbrev_len);
		//sb->setLen(sb->len + r);
		//}
		
		void strbuf_addIndentedText(StringBuffer *sb, const char *text,
			   	int indent, int indent2) {
			if (indent < 0) {
				indent = 0;
			}
			while (*text) {
				const char *eol = strchrnul(text, '\n');
				if (*eol == '\n') {
					eol++;
				}
				sb->addchars(' ', indent);
				sb->add(text, eol - text);
				text = eol;
				indent = indent2;
			}
		}

		size_t strbuf_fread(StringBuffer *sb, ssize_t size, FILE *f) {
			size_t oldalloc = sb->getAlloc();

			sb->grow(size);
			size_t res = fread(sb->buf + sb->len, 1, size, f);
			if (res > 0) {
				sb->setLen(sb->len + res);
			} else if (oldalloc == 0) {
				sb->release();
			}
			return res;
		}

		ssize_t strbuf_readFile(StringBuffer *sb, int fd, size_t hint) {
			size_t oldlen = sb->len;
			size_t oldalloc = sb->getAlloc();

			sb->grow(hint ? hint : 8192);
			for (;;) {
				ssize_t want = sb->getAlloc() - sb->len - 1;
				ssize_t got = read_in_full(fd, sb->buf + sb->len, want);

				if (got < 0) {
					if (oldalloc == 0) {
						sb->release();
					} else {
						sb->setLen(oldlen);
					}
					return -1;
				}
				sb->len += got;
				if (got < want) {
					break;
				}
				sb->grow(8192);
			}

			sb->buf[sb->len] = '\0';
			return sb->len - oldlen;
		}

		ssize_t strbuf_readFileByPath(StringBuffer *sb, const char *path, size_t hint) {
			int fd = open(path, O_RDONLY);
			if (fd < 0) {
				return -1;
			}
			ssize_t rlen = strbuf_readFile(sb, fd, hint);
			close(fd);
			if (rlen < 0) {
				return -1;
			}

			return rlen;
		}

		ssize_t strbuf_readOnce(StringBuffer *sb, int fd, size_t hint) {
			sb->grow(hint ? hint : 8192);
			ssize_t cnt = xread(fd, sb->buf + sb->len, sb->getAlloc() - sb->len - 1);
			if (cnt > 0) {
				sb->setLen(sb->len + cnt);
			}
			return cnt;
		}

		/*
		 * strbuf_write
		 */
		ssize_t strbuf_write(StringBuffer *sb, FILE *f) {
			return sb->len ? fwrite(sb->buf, 1, sb->len, f) : 0;
		}

		/*
		 * strbuf_readlink
		 */
		int strbuf_readlink(StringBuffer *sb, const char *path, size_t hint) {
			size_t oldalloc = sb->getAlloc();

			if (hint < 32) {
				hint = 32;
			}

			while (hint < STRBUF_MAXLINK) {
				sb->grow(hint);
				int newLen = readlink(path, sb->buf, hint);
				if (newLen < 0) {
					if (errno != ERANGE) {
						break;
					}
				} else if (newLen < hint) {
					sb->setLen(newLen);
					return 0;
				}

				/* .. the buffer was too small - try again */
				hint *= 2;
			}
			if (oldalloc == 0) {
				sb->release();
			}
			return -1;
		}

	}
}
