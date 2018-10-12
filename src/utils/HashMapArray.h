/**
 * Array list of hash maps
 */

#ifndef SRC_UTILS_HASHMAPARRAY_H
#define SRC_UTILS_HASHMAPARRAY_H

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <exception>
#include <iostream>
#include <stdexcept>

#include "utils/R3DMatrix.h"
#include "utils/hash/HashEngine.h"

namespace utils {

template <typename T>
class ValueUpdater {
    public:
    virtual void update(T& oldValue, const T& newValue,
                        const size_t valueSizeof) const = 0;
};

template <typename K, typename V>
struct MapEntry {
    K key;
    V value;
    friend std::ostream& operator<<(std::ostream& os, const MapEntry<K, V>& val)
    {
        os << "key {" << val.key << "}, value {" << val.value << "}";
        return os;
    }
};

template <typename K, typename V>
class HashMapArray {
    public:
    typedef typename utils::R3DMatrix<bool, bool, MapEntry<K, V>>::Cell Cell;

    HashMapArray(size_t rowSize, hash::HashEngine<K>* _hashEngine)
        : hashEngine(_hashEngine), valueSizeof(sizeof(V))
    {
        matrix = new utils::R3DMatrix<bool, bool, MapEntry<K, V>>(rowSize, 0);
    }

    HashMapArray<K, V>(const HashMapArray<K, V>&) =
        delete; // disable copy constructor
    HashMapArray<K, V>&
    operator=(const HashMapArray<K, V>&) = delete; // assign operator
    HashMapArray<K, V>&
    operator=(HashMapArray<K, V>&&) = delete; // replacement operator

    ~HashMapArray()
    {
        delete matrix;
        matrix = nullptr;
    }

    bool operator==(const HashMapArray<K, V>& other) const
    {
        if ((*other.getMatrix()) != (*matrix)) {
            return false;
        }
        return true;
    }

    bool operator!=(const HashMapArray<K, V>& other) const
    {
        return !((*this) == other);
    }

    utils::R3DMatrix<bool, bool, MapEntry<K, V>>* getMatrix() const
    {
        return matrix;
    }

    bool pushValue(size_t rowIndex, const K& key, const V& value,
                   const ValueUpdater<V>* updater = nullptr)
    {
        size_t colIndex = hashEngine->hashIndex(key);
        MapEntry<K, V> entry = {key, value};

        if (rowIndex < matrix->getRowSize()) {
            if (colIndex < (*matrix)[rowIndex].cellSize) {
                // пройти по точкам с одинаковым хэшкодом (значения коллизий)
                Cell& cell = (*matrix)[rowIndex][colIndex];
                for (size_t i = 0; i < cell.pointSize; ++i) {
                    MapEntry<K, V>& oldEntry = cell[i];
                    if (oldEntry.key == key) {
                        // найдено значение с уже имеющимся ключом - обновить
                        if (updater) {
                            // custom update function
                            updater->update(oldEntry.value, value, valueSizeof);
                        }
                        else {
                            // simple replace value
                            std::memcpy(&(oldEntry.value), &value, valueSizeof);
                        }
                        return false;
                    }
                }
                // key not found - add entry to end
                matrix->writePoints(rowIndex, colIndex, &entry, 1, true);
                return true;
            }
            else {
                // re-initialize cols memory
                matrix->writePoints(rowIndex, colIndex, &entry, 1);
                return true;
            }
        }
        else {
            // re-initialize row memory
            matrix->writePoints(rowIndex, colIndex, &entry, 1);
            return true;
        }
    }

    V* getValue(size_t rowIndex, const K& key)
    {
        if (rowIndex < matrix->getRowSize()) {
            size_t colIndex = hashEngine->hashIndex(key);
            if (colIndex < (*matrix)[rowIndex].cellSize) {
                // пройти по точкам с одинаковым хэшкодом (значения коллизий)
                Cell& cell = (*matrix)[rowIndex][colIndex];
                for (size_t i = 0; i < cell.pointSize; ++i) {
                    MapEntry<K, V>& entry = cell[i];
                    if (entry.key == key) {
                        return &(entry.value);
                    }
                }
                return nullptr;
            }
            else {
                return nullptr;
            }
        }
        else {
            return nullptr;
        }
    }

    private:
    utils::R3DMatrix<bool, bool, MapEntry<K, V>>* matrix;
    hash::HashEngine<K>* hashEngine;

    size_t valueSizeof; // saved value sizeof
};
} // namespace utils

#endif
