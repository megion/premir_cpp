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

		//void addUniqueAbbrev(StringBuffer *sb, const unsigned char *sha1, int abbrev_len) {
		//sb->grow(GIT_SHA1_HEXSZ + 1);
		//int r = find_unique_abbrev_r(sb->buf + sb->len, sha1, abbrev_len);
		//sb->setLen(sb->len + r);
		//}

	}
}
