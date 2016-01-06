#ifndef SRC_UTILS_CHASHSET_H
#define SRC_UTILS_CHASHSET_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

/**
 * Hash set only for store
 */
namespace utils {

    template<typename T, typename H>
    class CHashSet {
    public:

        // int, float (without range), string
        CHashSet() :
                tableSize(DEFAULT_TABLE_SIZE), hashEngine(tableSize) {
        }

        // float with range
        CHashSet(H _hashEngine) :
                tableSize(DEFAULT_TABLE_SIZE), hashEngine(_hashEngine) {
            hashEngine.setIndexSize(tableSize);
        }

        CHashSet<T, H>(const CHashSet<T, H> &) = delete; // disable copy constructor
        CHashSet<T, H> &operator=(const CHashSet<T, H> &) = delete; // assign operator
        CHashSet<T, H> &operator=(CHashSet<T, H> &&) = delete; // replacement operator

        ~CHashSet() {
        }

        size_t hash(const T& value) const {
            return hashEngine.hashCode(value);
        }


    private:

        size_t tableSize;
        // наилучшее случайное распределние занений хэш функции получается,
        // если размер таблицы индексов равен простому числу
        const static size_t DEFAULT_TABLE_SIZE = 1021;


        H hashEngine; // hash code generator

    };
}

#endif
