//
// Created by ilya 
// hash code for type array of T  
//

#ifndef SRC_UTILS_HASH_ARRAYLISTHASH_H
#define SRC_UTILS_HASH_ARRAYLISTHASH_H

#include <cstdio>
#include <typeinfo>

#include "utils/hash/HashEngine.h"
#include "utils/CArrayList.h"

namespace utils {
    namespace hash {

        template<typename T>
        class ArrayListHash : public HashEngine<typename utils::CArrayList<T>> {
        public:
			typedef typename utils::CArrayList<T> List;

            ArrayListHash(HashEngine<T> *_elementArrayHashEngine) :
			   	HashEngine<List>::HashEngine(),
				elementArrayHashEngine(_elementArrayHashEngine) {
            }

		protected:
            size_t hashCode(const List& value) const {
				// java ArrayList hashCode
				//int hashCode = 1;
				//for (E e : this)
					//hashCode = 31*hashCode + (e==null ? 0 : e.hashCode());
				//return hashCode;
				return 0;
            }

		private:
			HashEngine<T> *elementArrayHashEngine;

        };

		/**
		 * Hash code for type array of simple type.
		 * Use only for simple type (char, int, short, double and etc.) 
		 * because for Class or struct it may calculate different hash for same objects 
		 */
		template<typename T>
        class ArrayListSimpleTypeHash : public HashEngine<typename utils::CArrayList<T>> {
        public:
			typedef typename utils::CArrayList<T> List;

            ArrayListSimpleTypeHash() : HashEngine<List>::HashEngine() {
				if (typeid(T)==typeid(int)) {
				}

				if (typeid(T)==typeid(size_t)) {
				}
				if (typeid(T)==typeid(long)) {
				}
            }

		protected:
            size_t hashCode(const List& value) const {
				// use DEKHash here
				return 0;
            }

        };

    }
}


#endif
