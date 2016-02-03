#ifndef SRC_UTILS_HASH_FLOATHASH_H
#define SRC_UTILS_HASH_FLOATHASH_H

#include <cstdio>

#include "HashEngine.h"

namespace utils {
    namespace hash {

        template<typename T>
        class FloatHash : public HashEngine<T> {
        public:
            FloatHash(double _start, double _end) :
                    start(_start), end(_end) {
            }

            size_t hashCode(const T &value) const {
                return (size_t) HashEngine<T>::indexSize * (value - start) /
                       (end - start);
            }

        private:

            double start;
            double end;
        };
    }
}


#endif
