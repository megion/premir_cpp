//
// Created by ilya 
// hash code for type array of T  
//

#ifndef SRC_UTILS_HASH_ARRAYLISTHASH_H
#define SRC_UTILS_HASH_ARRAYLISTHASH_H

#include <cstdio>

#include "utils/hash/HashEngine.h"
#include "utils/CArrayList.h"

namespace utils {
    namespace hash {

        template<typename T>
        class ArrayListHash : public HashEngine<typename utils::CArrayList<T>> {
        public:
			typedef typename utils::CArrayList<T> List;

            ArrayListHash() : HashEngine<List>::HashEngine() {
            }

            size_t hash(const List& value) const {
				//size_t code = hashArray(value.getArray(), value.size());
                //return code % HashEngine<List>::indexSize;
				return 0;
            }

        };
    }
}


#endif
