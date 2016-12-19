#ifndef SRC_UTILS_HASH_HASHENGINE_H
#define SRC_UTILS_HASH_HASHENGINE_H

#include <cstdio>
#include <cstring>

namespace utils {
    namespace hash {

        template<typename T>
        class HashEngine {
        public:

            HashEngine(size_t _indexSize=DEFAULT_TABLE_SIZE) : indexSize(_indexSize),
			doubleSizeof(sizeof(double)) {
            }

            virtual size_t hashCode(const T& value) const = 0;

            void setIndexSize(size_t _indexSize) {
                indexSize = _indexSize;
            }

			size_t hash(const double& value) {
				char temp[doubleSizeof];
				std::memcpy(temp, &value, doubleSizeof);
			}

        protected:
            size_t indexSize;

			size_t doubleSizeof;
			
			/**
			 * An algorithm proposed by Donald E. Knuth in The Art Of Computer Programming Volume 3,
			 * under the topic of sorting and search chapter 6.4.
			 */
			size_t DEKHash(const char* str, const size_t& len) {
				size_t hash = len;
				for(size_t i = 0; i < len; str++, i++) {
					hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
				}
				return hash;
			}
			
			size_t strHash(const char* str, const size_t& len) {
				return DEKHash(str, len);
			}

        public:
            // наилучшее случайное распределние занений хэш функции получается,
            // если размер таблицы индексов равен простому числу
            const static size_t DEFAULT_TABLE_SIZE = 1021;

        };
    }
}


#endif
