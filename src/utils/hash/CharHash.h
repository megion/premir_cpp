//
// Created by ilya.
//

#ifndef SRC_UTILS_HASH_CHARHASH_H
#define SRC_UTILS_HASH_CHARHASH_H

#include <cstdio>

#include "HashEngine.h"

namespace utils {
    namespace hash {

		class CharHash : public HashEngine<char> {
		public:
			CharHash() : HashEngine<char>::HashEngine(255) {
			}

		protected:
			size_t hashCode(const char &value) const {
				// simple cast to size_t type
				return (size_t)value;
			}
		};
    }
}


#endif
