#ifndef SRC_UTILS_CHASHSET_H
#define SRC_UTILS_CHASHSET_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include "hash/HashEngine.h"

/**
 * Hash set only for store
 */
namespace utils {

    template<typename T>
    class CHashSet {
    public:

        // int, float (without range), string
//        CHashSet() :
//                tableSize(DEFAULT_TABLE_SIZE), hashEngine(tableSize) {
//        }

        // float with range
        CHashSet(hash::HashEngine<T>* _hashEngine) :
                tableSize(DEFAULT_TABLE_SIZE), hashEngine(_hashEngine) {
            hashEngine->setIndexSize(tableSize);
        }

        // copy constructor
        CHashSet(const CHashSet<T> &) = delete;

        // replace constructor
        CHashSet(CHashSet<T> &&) = delete;

        // assign operator
        CHashSet<T> &operator=(const CHashSet<T> &) = delete;

        // replacement operator
        CHashSet<T> &operator=(CHashSet<T> &&) = delete;

        ~CHashSet() {
        }

        size_t hash(const T &value) const {
            return hashEngine->hashCode(value);
        }


    private:

        size_t tableSize;
        // наилучшее случайное распределние занений хэш функции получается,
        // если размер таблицы индексов равен простому числу
        const static size_t DEFAULT_TABLE_SIZE = 1021;


        hash::HashEngine<T>* hashEngine; // hash code generator

    };
}

#endif
