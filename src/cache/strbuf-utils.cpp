#include "cache/strbuf-utils.h"

namespace cache {

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
}


