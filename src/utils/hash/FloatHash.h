//
// Created by ilya.
// FloatHash for any float value.
// For example: float, double 
//

#ifndef SRC_UTILS_HASH_FLOATHASH_H
#define SRC_UTILS_HASH_FLOATHASH_H

#include <cstdio>

#include "HashEngine.h"

namespace utils {
    namespace hash {

		template<typename T>
		class FloatHash : public HashEngine<T> {
		public:
			FloatHash() : 
				HashEngine<T>::HashEngine(),
				typeSizeof(sizeof(T))	{
			}

		protected:
			size_t hashCode(const T &value) const {
				char temp[typeSizeof];
				std::memcpy(temp, &value, typeSizeof);
				return charsHash(temp, typeSizeof);
			}

		private:
			size_t typeSizeof;	
		};

	}
}


#endif
