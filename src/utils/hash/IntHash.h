//
// Created by ilya.
// IntHash for any integer value.
// For example: int, short, long, size_t
//

#ifndef SRC_UTILS_HASH_INTHASH_H
#define SRC_UTILS_HASH_INTHASH_H

#include <cstdio>

#include "HashEngine.h"

namespace utils {
    namespace hash {

		template<typename T>
		class IntHash : public HashEngine<T> {
		public:
			IntHash() : HashEngine<T>::HashEngine() {
			}

		protected:
			size_t hashCode(const T &value) const {
				// simple cast to size_t type
				return (size_t)value;
			}
		};


	}
}


#endif
