//
// Created by ilya on 1/6/16.
// hash code for float number value if float range is defined
//

#ifndef SRC_UTILS_HASH_FLOATHASH_H
#define SRC_UTILS_HASH_FLOATHASH_H

#include <cstdio>

namespace utils {
    namespace hash {

        template<typename T>
        class FloatHash {
        public:
            FloatHash(double _start, double _end) :
                    indexSize(0), start(_start), end(_end) {
            }

            size_t setIndexSize(size_t _indexSize) {
                indexSize = _indexSize;
            }

            size_t hashCode(const T& value) const {
                return (size_t) indexSize * (value - start) /
                   (end - start);
            }

        private:
            size_t indexSize;

            double start;
            double end;
        };
    }
}


#endif
