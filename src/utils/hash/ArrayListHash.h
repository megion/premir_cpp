//
// Created by ilya 
// hash code for type array of T  
//

#ifndef SRC_UTILS_HASH_ARRAYLISTHASH_H
#define SRC_UTILS_HASH_ARRAYLISTHASH_H

#include <cstdio>
#include <typeinfo>

#include "utils/console_colors.h"
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
				size_t hash = 1;
				for (size_t i = 0; i<value.size(); i++) {
					hash = 31*hash + elementArrayHashEngine->hashIndex(value[i]);
				}
				return hash;
            }

		private:
			HashEngine<T> *elementArrayHashEngine;

        };

		/**
		 * Hash code for type array of simple type.
		 * Use only for simple type (char, int, short, double and etc.) 
		 * It is very fast implementations
		 */
		template<typename T>
        class ArrayListSimpleTypeHash : public HashEngine<typename utils::CArrayList<T>> {
        public:
			typedef typename utils::CArrayList<T> List;

            ArrayListSimpleTypeHash() : HashEngine<List>::HashEngine() {
				checkType();	
            }

			ArrayListSimpleTypeHash(size_t _indexSize) : HashEngine<List>::HashEngine(_indexSize) {
				checkType();
			}

		protected:
            size_t hashCode(const List& value) const {
				// use DEKHash here
				return DEKHash<T>(value.getArray(), value.size());
            }

		private:
			void checkType() const {
				danger_text("error: some error occurs");
				PRINT_LOG(ERR, "type not supported: %s\n", typeid(T).name());
				if (typeid(T)==typeid(int)) {
				}
				if (typeid(T)==typeid(size_t)) {
				}
				if (typeid(T)==typeid(short)) {
				}
			}

        };

    }
}


#endif
