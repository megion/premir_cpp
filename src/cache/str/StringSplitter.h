#ifndef SRC_CACHE_STR_STRING_SPLITTER_H
#define SRC_CACHE_STR_STRING_SPLITTER_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include "cache/str/StringBuffer.h"

namespace cache {
namespace str {

class StringSplitter {
  public:
    StringSplitter() : array(nullptr), nr(0), alloc(0) {}

    ~StringSplitter() { release(); }

    void release();
    /*
     * create array of pointer on StringBuffer.
     * Last array element is nullptr.
     */
    StringBuffer** splitBuf(const char* str, size_t slen, int terminator,
                            size_t max);

  private:
    StringBuffer** array;
    size_t nr, alloc;

    void grow(size_t newLen) {
        if (newLen > alloc) {
            size_t newAlloc = (alloc + 16) * 3 / 2;
            if (newAlloc < newLen) {
                alloc = newLen;
            } else {
                alloc = newAlloc;
            }
            reallocArray();
        }
    }

    void reallocArray() {
        size_t amount = sizeof(StringBuffer*) * alloc;
        StringBuffer** newArray = (StringBuffer**)std::realloc(array, amount);

        if (newArray == nullptr) {
            throw std::runtime_error(std::strerror(errno));
        }
        array = newArray;
    }
};
}
}

#endif
