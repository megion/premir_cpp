/**
 * Hash maps
 */

#ifndef SRC_UTILS_HASHMAP_H
#define SRC_UTILS_HASHMAP_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include "utils/RDMatrix.h"
#include "utils/hash/HashEngine.h"
#include "HashMapArray.h"

namespace utils {


    template<typename K, typename V>
    class HashMap {
    public:

        typedef typename utils::RDMatrix<bool, MapEntry<K, V>>::Row Row;

        HashMap(hash::HashEngine<K> *_hashEngine) :
                hashEngine(_hashEngine), valueSizeof(sizeof(V)) {
            matrix = new utils::RDMatrix<bool, MapEntry<K, V>>();
        }

        HashMap<K, V>(const HashMap<K, V> &) = delete; // disable copy constructor
        HashMap<K, V> &operator=(const HashMap<K, V> &) = delete; // assign operator
        HashMap<K, V> &operator=(HashMap<K, V> &&) = delete; // replacement operator

        ~HashMap() {
            delete matrix;
            matrix = nullptr;
        }

        bool operator==(const HashMap<K, V> &other) const {
            if ((*other.getMatrix()) != (*matrix)) {
                return false;
            }
            return true;
        }

        bool operator!=(const HashMap<K, V> &other) const {
            return !((*this) == other);
        }

        utils::RDMatrix<bool, MapEntry<K, V>> *getMatrix() const {
            return matrix;
        }

        bool pushValue(const K &key, const V &value, const ValueUpdater<V> *updater = nullptr) {
            size_t rowIndex = hashEngine->hashIndex(key);
            MapEntry<K, V> entry = {key, value};

            if (rowIndex < matrix->getRowSize()) {
                // пройти по точкам с одинаковым хэшкодом (значения коллизий)
                Row &row = (*matrix)[rowIndex];
                for (size_t i = 0; i < row.pointSize; ++i) {
					MapEntry<K, V>& oldEntry = row[i];
                    if (oldEntry.key == key) {
                        // найдено значение с уже имеющимся ключом - обновить
                        if (updater) {
                            // custom update function
                            updater->update(oldEntry.value, value, valueSizeof);
                        } else {
                            // simple replace value
                            std::memcpy(&(oldEntry.value), &value, valueSizeof);
                        }
                        return false;
                    }
                }
                // key not found - add entry to end
                matrix->writeToEndRow(rowIndex, entry);
                return true;
            } else {
                matrix->writeRow(rowIndex, &entry, 1);
                return true;
            }

        }

        V *getValue(const K &key) {
            size_t rowIndex = hashEngine->hashIndex(key);
            if (rowIndex < matrix->getRowSize()) {
                // пройти по точкам с одинаковым хэшкодом (значения коллизий)
                Row &row = (*matrix)[rowIndex];
                for (size_t i = 0; i < row.pointSize; ++i) {
                    if (row[i].key == key) {
                        return &row[i].value;
                    }
                }
                return nullptr;
            } else {
                return nullptr;
            }
        }


    private:
        utils::RDMatrix<bool, MapEntry<K, V>> *matrix;
        hash::HashEngine<K> *hashEngine;

        size_t valueSizeof; // saved value sizeof 

    };
}


#endif
