#include "cache/strbuf.h"

namespace cache {
	
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


