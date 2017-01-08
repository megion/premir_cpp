#ifndef SRC_UTILS_HASH_HASHENGINE_H
#define SRC_UTILS_HASH_HASHENGINE_H

#include <cstdio>
#include <cstring>

namespace utils {
    namespace hash {

        template<typename T>
        class HashEngine {
        public:

            HashEngine(size_t _indexSize=DEFAULT_TABLE_SIZE) :
			   	indexSize(_indexSize),
				doubleSizeof(sizeof(double)),
				floatSizeof(sizeof(float)),
				intSizeof(sizeof(int)), 
				shortSizeof(sizeof(short)),
				tSizeof(sizeof(short)) {
            }

            virtual size_t hashCode(const T& value) const = 0;

            void setIndexSize(size_t _indexSize) {
                indexSize = _indexSize;
            }

			/**
			 * double 
			 */
			size_t hash(const double& value) const {
				char temp[doubleSizeof];
				std::memcpy(temp, &value, doubleSizeof);
				return hash(temp, doubleSizeof);
			}

			/**
			 * float 
			 */
			size_t hash(const float& value) const {
				char temp[floatSizeof];
				std::memcpy(temp, &value, floatSizeof);
				return hash(temp, floatSizeof);
			}

			/**
			 * int 
			 */
			size_t hash(const int& value) const {
				char temp[intSizeof];
				std::memcpy(temp, &value, intSizeof);
				return hash(temp, intSizeof);
			}

			/**
			 * short 
			 */
			size_t hash(const short& value) const {
				char temp[shortSizeof];
				std::memcpy(temp, &value, shortSizeof);
				return hash(temp, shortSizeof);
			}

			/**
			 * bool 
			 */
			size_t hash(const bool& value) const {
				// like java jdk
				return value ? 1231 : 1237;
			}

			size_t hashArray(const T* values, const size_t& len) {
				size_t arrSizeof = len * tSizeof;
				char temp[arrSizeof];
				std::memcpy(temp, values, arrSizeof);
				return hash(temp, arrSizeof);
			}

			size_t hash(const char* str, const size_t& len) const {
				return DEKHash(str, len);
			}

        protected:
            size_t indexSize;

			size_t doubleSizeof;
			size_t floatSizeof;	
			size_t intSizeof;	
			size_t shortSizeof;
			size_t tSizeof;

			/**
			 * An algorithm proposed by Donald E. Knuth in The Art Of Computer Programming Volume 3,
			 * under the topic of sorting and search chapter 6.4.
			 */
			size_t DEKHash(const char* str, const size_t& len) const {
				size_t hash = len;
				for(size_t i = 0; i < len; str++, i++) {
					hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
				}
				return hash;
			}
			
        public:
            // наилучшее случайное распределние занений хэш функции получается,
            // если размер таблицы индексов равен простому числу
            const static size_t DEFAULT_TABLE_SIZE = 1021;

        };
    }
}


#endif
