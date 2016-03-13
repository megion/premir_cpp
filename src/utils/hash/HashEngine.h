#ifndef SRC_UTILS_HASH_HASHENGINE_H
#define SRC_UTILS_HASH_HASHENGINE_H

#include <cstdio>

namespace utils {
    namespace hash {

        template<typename T>
        class HashEngine {
        public:

            HashEngine(size_t _indexSize=DEFAULT_TABLE_SIZE) : indexSize(_indexSize) {
            }

            virtual size_t hashCode(const T& value) const = 0;

            size_t setIndexSize(size_t _indexSize) {
                indexSize = _indexSize;
            }

        protected:
            size_t indexSize;

        public:
            // наилучшее случайное распределние занений хэш функции получается,
            // если размер таблицы индексов равен простому числу
            const static size_t DEFAULT_TABLE_SIZE = 1021;

        };
    }
}


#endif
