#include "cache/strbuf-utils.h"

namespace cache {

	StringBuffer **splitBuf(const char *str, size_t slen,
			int terminator, int max) {
		StringBuffer **ret = nullptr;
		size_t nr = 0, alloc = 0;
		StringBuffer *t;

		while (slen) {
			int len = slen;
			if (max <= 0 || nr + 1 < max) {
				const char *end = memchr(str, terminator, slen);
				if (end)
					len = end - str + 1;
			}
			t = new StringBuffer();
			//t = xmalloc(sizeof(struct strbuf));
			//strbuf_init(t, len);
			t->add(str, len);
			//strbuf_add(t, str, len);
			ALLOC_GROW(ret, nr + 2, alloc);
			ret[nr++] = t;
			str += len;
			slen -= len;
		}
		ALLOC_GROW(ret, nr + 1, alloc); /* In case string was empty */
		ret[nr] = nullptr;
		return ret;
	}
}


