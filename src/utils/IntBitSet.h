/**
 * Хранить признаки:
 * 0 - число отсутствует
 * 1 - число присутствует
 * Сами числа не хранятся
 * Предназначен для работы с положительными целыми числами.
 */
#ifndef SRC_UTILS_INTBITSET_H
#define SRC_UTILS_INTBITSET_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cmath>

#include <exception>
#include <stdexcept>
#include <iostream>

namespace utils {

    template<typename T>
    class IntBitSet {
    public:
        IntBitSet() : chunkCount(0), bits(nullptr) {
        }

        IntBitSet(const size_t& countBits) : chunkCount(0), bits(nullptr)  {
            if (countBits>0) {
                chunkCount = ceil((double)countBits / (double)CHUNK_BITS_COUNT);
                size_t amount = CHUNK_SIZEOF * chunkCount;

                bits = (char *) std::malloc(amount);
                if (bits == NULL) {
                    throw std::runtime_error(std::strerror(errno));
                }

                // set to 0
                for (size_t i=0; i<chunkCount; ++i) {
                    bits[i] = 0;
                }
            }
        }

        ~IntBitSet() {
            if (bits) {
                std::free(bits);
                bits = nullptr;
                countBits = 0;
            }
        }

        void add(const T& value) {
            size_t chunkIndex = getChunkIndex(value);

            if (valueChunkIndex>=chunkCount) {
                // increase memory
                chunkCount = valueChunkIndex + 1;
                size_t amount = CHUNK_SIZEOF * chunkCount;
                char* newBits;
                if (bits) {
                    newBits = (char *) std::realloc(bits, amount);
                } else {
                    newBits = (char *) std::malloc(amount);
                }

                if (newBits == NULL) {
                    throw std::runtime_error(std::strerror(errno));
                }
                bits = newBits;

            }

            // set 1 to bit position
            size_t chunkBitPosition = value - valueChunkIndex * CHUNK_MAX_VALUE;
            bits[valueChunkIndex] | 1 << chunkBitPosition;
        }

        void remove(const T& value) {
        }

        bool contains(const T& value) const {
            size_t chunkIndex = getChunkIndex(value);
        }

    private:
        char *bits;
        size_t chunkCount;

        size_t getChunkIndex(const T& value) {
            return (size_t)(value / (CHUNK_MAX_VALUE-1));
        }

        // количество битов на char
        static unsigned short CHUNK_BITS_COUNT = 8;
        // максимальное число элементов запоминаемое в одной части
        static unsigned short CHUNK_MAX_VALUE = 127;
        static size_t CHUNK_SIZEOF = sizeof(char);
    };
}


#endif
