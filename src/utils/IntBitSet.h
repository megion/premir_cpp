/**
 * Хранить признаки:
 * 0 - число отсутствует
 * 1 - число присутствует
 * Сами числа не хранятся
 * Предназначен для работы с положительными целыми числами.
 * Реализация не эффективна при работе большими числами.
 */
#ifndef SRC_UTILS_INTBITSET_H
#define SRC_UTILS_INTBITSET_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

namespace utils {

    template<typename T>
    class IntBitSet {
    public:
        IntBitSet() : charCount(0), bits(nullptr) {
        }

        IntBitSet(const size_t &countBits) : charCount(0), bits(nullptr) {
            if (countBits > 0) {
                charCount = std::ceil(
                        (double) countBits / (double) CHAR_BITS_COUNT);
                size_t amount = CHAR_SIZEOF * charCount;

                bits = (char *) std::malloc(amount);
                if (bits == NULL) {
                    throw std::runtime_error(std::strerror(errno));
                }

                // set all to 0
                for (size_t i = 0; i < charCount; ++i) {
                    bits[i] = 0;
                }
            }
        }

        ~IntBitSet() {
            if (bits) {
                std::free(bits);
                bits = nullptr;
                charCount = 0;
            }
        }

        void add(const T &value) {
            size_t charIndex = getCharIndex(value);

            if (charIndex >= charCount) {
                // increase memory
                size_t oldCharCount = charCount;
                charCount = charIndex + 1;
                size_t amount = CHAR_SIZEOF * charCount;
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
                for (size_t i = oldCharCount; i < charCount; ++i) {
                    bits[i] = 0;
                }
            }

            // set 1 to bit position
            char& ch = bits[charIndex];
            ch = ch | (1 << getCharBitIndex(value, charIndex));
        }

        void remove(const T &value) {
            size_t charIndex = getCharIndex(value);
            if (charIndex >= charCount) {
                return;
            }
            char& ch = bits[charIndex];

            // changing the nth bit to x
            // number ^= (-x ^ number) & (1 << n);
            ch ^= ch & (1 << getCharBitIndex(value, charIndex));
        }

        bool contains(const T &value) const {
            size_t charIndex = getCharIndex(value);
            if (charIndex >= charCount) {
                return false;
            }

            char& ch = bits[charIndex];
            if ( ((ch >> getCharBitIndex(value, charIndex)) & 1) == 1) {
                return true;
            } else {
                return false;
            }
        }

        void print() {
            for (size_t i = 0; i < charCount; ++i) {
                for (size_t bit = 0; bit < CHAR_BITS_COUNT; ++bit) {
                    std::cout << (((bits[i]) >> bit) & 1);
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }

    private:
        char *bits;
        size_t charCount;

        size_t getCharIndex(const T &value) const {
            return (size_t) (value / CHAR_BITS_COUNT);
        }

        size_t getCharBitIndex(const T &value, const size_t &charIndex) const {
            return value - CHAR_BITS_COUNT * charIndex;
        }

        // число байтов на одной части
        const static size_t CHAR_SIZEOF = sizeof(char);

        // количество битов в части
        const static size_t CHAR_BITS_COUNT = 8 * CHAR_SIZEOF;

    };
}


#endif
