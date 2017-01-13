//
// Created by ilya 
// hash code for type array of simple type.
// Use only for simple type (char, int, short, double and etc.) 
// because for Class or struct it may calculate different hash for same objects 
//

#ifndef SRC_UTILS_HASH_ARRAYLIST_SIMPLETYPE_HASH_H
#define SRC_UTILS_HASH_ARRAYLIST_SIMPLETYPE_HASH_H

#include <cstdio>
#include <typeinfo>

#include "utils/hash/HashEngine.h"
#include "utils/CArrayList.h"

namespace utils {
    namespace hash {

        template<typename T>
        class ArrayListSimpleTypeHash : public HashEngine<typename utils::CArrayList<T>> {
        public:
			typedef typename utils::CArrayList<T> List;

            ArrayListSimpleTypeHash() : HashEngine<List>::HashEngine() {
				if (typeid(T)==typeid(int)) {
				}
            }

		protected:
            size_t hashCode(const List& value) const {
				return 0;
            }

        };
    }
}


#endif
