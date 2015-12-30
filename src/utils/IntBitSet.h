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

        IntBitSet(const size_t &countBits) : chunkCount(0), bits(nullptr) {
            if (countBits > 0) {
                chunkCount = std::ceil(
                        (double) countBits / (double) CHUNK_BITS_COUNT);
                size_t amount = CHUNK_SIZEOF * chunkCount;

                bits = (char *) std::malloc(amount);
                if (bits == NULL) {
                    throw std::runtime_error(std::strerror(errno));
                }

                // set to 0
                for (size_t i = 0; i < chunkCount; ++i) {
                    bits[i] = 0;
                }
            }
        }

        ~IntBitSet() {
            if (bits) {
                std::free(bits);
                bits = nullptr;
                chunkCount = 0;
            }
        }

        void add(const T &value) {
            size_t chunkIndex = getChunkIndex(value);

            if (chunkIndex >= chunkCount) {
                // increase memory
                size_t oldChunkCount = chunkCount;
                chunkCount = chunkIndex + 1;
                size_t amount = CHUNK_SIZEOF * chunkCount;
                char *newBits;
                if (bits) {
                    newBits = (char *) std::realloc(bits, amount);
                } else {
                    newBits = (char *) std::malloc(amount);
                }

                if (newBits == NULL) {
                    throw std::runtime_error(std::strerror(errno));
                }
                bits = newBits;

                // set 0 for new bits
                for (size_t i = oldChunkCount; i < chunkCount; ++i) {
                    bits[i] = 0;
                }
            }

            // set 1 to bit position
//            size_t chunkBitPosition = value - chunkIndex * CHUNK_MAX_VALUE;
            std::cout << "(1 << 3): " << (1 << 1) << std::endl;
            char& ch = bits[0];
            ch = ch | (1 << 7);
        }

        void remove(const T &value) {
        }

        bool contains(const T &value) const {
            size_t chunkIndex = getChunkIndex(value);
        }

        void print() {
            std::cout << "CHUNK_BITS_COUNT: " << CHUNK_BITS_COUNT << std::endl;
            std::cout << "CHUNK_SIZEOF: " << CHUNK_SIZEOF << std::endl;
            std::cout << "CHUNK_MAX_VALUE: " << CHUNK_MAX_VALUE << std::endl;
            std::cout << "bits: " << bits << std::endl;

            for (size_t i = 0; i < chunkCount; ++i) {
//                bits[i] = 0;
                for (size_t bit = 0; bit < CHUNK_BITS_COUNT; ++bit) {
                    std::cout << (((bits[i]) >> bit) & 1);
                }
                std::cout << " ";
            }
            std::cout << " " << std::endl;
        }

    private:
        char *bits;
        size_t chunkCount;

        size_t getChunkIndex(const T &value) {
            return (size_t) (value / 7);
        }

        // число байтов в части
        const static size_t CHUNK_SIZEOF = sizeof(char);

        // количество битов в части
        const static size_t CHUNK_BITS_COUNT = 8 * CHUNK_SIZEOF;

        // максимальное число элементов запоминаемое в одной части
        const static size_t CHUNK_MAX_VALUE = std::pow(2, CHUNK_BITS_COUNT);

    };
}


#endif
