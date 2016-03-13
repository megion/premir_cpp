//
// Created by ilya on 1/6/16.
// hash code for char* or wchar_t* (string value)
//

#ifndef SRC_UTILS_HASH_STRINGHASH_H
#define SRC_UTILS_HASH_STRINGHASH_H

#include <cstdio>

#include "HashEngine.h"

namespace utils {
    namespace hash {

        template<typename T>
        class StringHash : public HashEngine<T> {
        public:
            StringHash() : HashEngine<T>::HashEngine() {
            }

            size_t hashCode(const T &value) const {
                size_t h = 0, a = 127, index = 0;
                for (; *(value + index) != 0; ++index) {
                    h = (a * h + *(value + index)) % HashEngine<T>::indexSize;
                }
                return h;
            }
        };


        class CharHash : public HashEngine<char> {
        public:
            CharHash() : HashEngine<char>::HashEngine(255) {
            }

            size_t hashCode(const char &value) const {
                return value % HashEngine<char>::indexSize;
            }
        };
    }
}


#endif
