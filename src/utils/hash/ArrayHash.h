//
// Created by ilya 
// hash code for type array of T  
//

#ifndef SRC_UTILS_HASH_ARRAYHASH_H
#define SRC_UTILS_HASH_ARRAYHASH_H

#include <cstdio>

#include "HashEngine.h"

namespace utils {
    namespace hash {

        template<typename HashEngine<T>>
        class NumberHash : public HashEngine<T> {
        public:
            NumberHash() : HashEngine<T>::HashEngine() {
            }

            size_t hashCode(const T& value) const {
                return ((size_t)value) % HashEngine<T>::indexSize;
            }


        };
    }
}


#endif
