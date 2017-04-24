#include "cache/str/StringSplitter.h"

namespace cache {
namespace str {

void StringSplitter::release() {
    if (array) {
        // StringBuffer **s = array;
        // while (*s) {
        // delete *s;
        //*s = nullptr;
        // s++;// TODO: diffrent from original src
        //}
        for (size_t i = 0; i < nr; i++) {
            delete array[i];
            array[i] = nullptr;
        }
        std::free(array);
        array = nullptr;
        alloc = 0;
        nr = 0;
    }
}

/*
 * create array of pointer on StringBuffer.
 * Last array element is nullptr.
 */
StringBuffer** StringSplitter::splitBuf(const char* str, size_t slen,
                                        int terminator, size_t max) {
    while (slen) {
        int len = slen;
        if (max <= 0 || nr + 1 < max) {
            const char* end = (const char*)memchr(str, terminator, slen);
            if (end) {
                len = end - str + 1;
            }
        }
        StringBuffer* t = new StringBuffer(len);
        t->add(str, len);
        grow(nr + 2);
        array[nr++] = t;
        str += len;
        slen -= len;
    }
    grow(nr + 1); /* In case string was empty */
    array[nr] = nullptr;
    return array;
}
}
}
