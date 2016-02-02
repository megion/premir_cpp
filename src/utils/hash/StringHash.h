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
            StringHash(size_t _indexSize) : indexSize(_indexSize) {
            }

            size_t setIndexSize(size_t _indexSize) {
                indexSize = _indexSize;
            }

            size_t hashCode(const T& value) const {
                T temp = value;
                size_t h = 0, a = 127;
                for (; *temp != 0; ++temp) {
                    h = (a * h + *temp) % indexSize;
                }
                return h;
            }

        private:
            size_t indexSize;
        };
    }
}


#endif
