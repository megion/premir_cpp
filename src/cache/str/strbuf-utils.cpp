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

		//void strbuf_expand(StringBuffer *sb, const char *format, expand_fn_t fn, void *context) {
			//for (;;) {
				//const char *percent;
				//size_t consumed;

				//percent = strchrnul(format, '%');
				//sb->add(format, percent - format);
				//if (!*percent) {
					//break;
				//}
				//format = percent + 1;

				//if (*format == '%') {
					//sb->addch('%');
					//format++;
					//continue;
				//}

				//consumed = fn(sb, format, context);
				//if (consumed) {
					//format += consumed;
				//} else {
					//sb->addch('%');
				//}
			//}
		//}

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

		int strbuf_getcwd(StringBuffer *sb) {
			size_t oldalloc = sb->getAlloc();
			size_t guessed_len = 128;

			for (;; guessed_len *= 2) {
				sb->grow(guessed_len);
				if (getcwd(sb->buf, sb->getAlloc())) {
					sb->setLen(std::strlen(sb->buf));
					return 0;
				}
				if (errno != ERANGE) {
					break;
				}
			}
			if (oldalloc == 0) {
				sb->release();
			} else {
				sb->reset();
			}
			return -1;
		}

		void strbuf_addstr_xml_quoted(StringBuffer *sb, const char *s) {
			while (*s) {
				size_t len = strcspn(s, "\"<>&");
				sb->add(s, len);
				s += len;
				switch (*s) {
					case '"':
						sb->addstr("&quot;");
						break;
					case '<':
						sb->addstr("&lt;");
						break;
					case '>':
						sb->addstr("&gt;");
						break;
					case '&':
						sb->addstr("&amp;");
						break;
					case 0:
						return;
				}
				s++;
			}
		}

		static void strbuf_add_urlencode(StringBuffer *sb, const char *s,
			   	size_t strlen, int reserved) {
			sb->grow(strlen);
			while (strlen--) {
				char ch = *s++;
				if (is_rfc3986_unreserved(ch) ||
						(!reserved && is_rfc3986_reserved(ch))) {
					sb->addch(ch);
				} else {
					sb->addf("%%%02x", ch);
				}
			}
		}

		void strbuf_addstr_urlencode(StringBuffer *sb, const char *s, int reserved) {
			strbuf_add_urlencode(sb, s, std::strlen(s), reserved);
		}

		void strbuf_humanise_bytes(StringBuffer *sb, off_t bytes) {
			if (bytes > 1 << 30) {
				sb->addf("%u.%2.2u GiB",
						(int)(bytes >> 30),
						(int)(bytes & ((1 << 30) - 1)) / 10737419);
			} else if (bytes > 1 << 20) {
				int x = bytes + 5243;  /* for rounding */
				sb->addf("%u.%2.2u MiB",
						x >> 20, ((x & ((1 << 20) - 1)) * 100) >> 20);
			} else if (bytes > 1 << 10) {
				int x = bytes + 5;  /* for rounding */
				sb->addf("%u.%2.2u KiB",
						x >> 10, ((x & ((1 << 10) - 1)) * 100) >> 10);
			} else {
				sb->addf("%u bytes", (int)bytes);
			}
		}

		char *xgetcwd(void) {
			StringBuffer sb;
			if (strbuf_getcwd(&sb)) {
				LOG(ERR, "unable to get current working directory");
			}
			return sb.detach(nullptr);
		}

		void strbuf_add_absolute_path(StringBuffer *sb, const char *path) {
			if (!*path) {
				LOG(ERR, "The empty string is not a valid path");
				return;
			}
			if (!is_absolute_path(path)) {
				struct stat cwd_stat, pwd_stat;
				size_t orig_len = sb->len;
				char *cwd = xgetcwd();
				char *pwd = getenv("PWD");
				if (pwd && strcmp(pwd, cwd) &&
						!stat(cwd, &cwd_stat) &&
						(cwd_stat.st_dev || cwd_stat.st_ino) &&
						!stat(pwd, &pwd_stat) &&
						pwd_stat.st_dev == cwd_stat.st_dev &&
						pwd_stat.st_ino == cwd_stat.st_ino) {
					sb->addstr(pwd);
				} else {
					sb->addstr(cwd);
				}
				if (sb->len > orig_len && !is_dir_sep(sb->buf[sb->len - 1])) {
					sb->addch('/');
				}
				std::free(cwd);
			}
			sb->addstr(path);
		}

		void strbuf_addftime(StringBuffer *sb, const char *fmt, const struct tm *tm) {
			size_t hint = 128;

			if (!*fmt) {
				return;
			}

			sb->grow(hint);
			size_t newlen = strftime(sb->buf + sb->len, sb->getAlloc() - sb->len, fmt, tm);

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
					sb->grow(hint);
					newlen = strftime(sb->buf + sb->len, sb->getAlloc() - sb->len,
							munged_fmt.buf, tm);
				}
				newlen--; /* drop munged space */
			}
			sb->setLen(sb->len + newlen);
		}

	}
}
