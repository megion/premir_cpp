/**
 * Array list of hash maps
 */

#ifndef SRC_UTILS_HASHMAPARRAY_H
#define SRC_UTILS_HASHMAPARRAY_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

namespace utils {

    template<typename K, typename V>
    class HashMapArray {
    public:

        typedef typename utils::R3DMatrix<bool, bool, Entry>::Row Row;
        typedef typename utils::R3DMatrix<bool, bool, Entry>::Cell Cell;

        struct Entry {
            K key;
            V value;
        }

        HashMapArray(hash::HashEngine<K>* _hashEngine, size_t _tableSize, size_t rowSize) :
                tableSize(_tableSize), hashEngine(_hashEngine) {
            hashEngine->setIndexSize(tableSize);
            matrix = new utils::R3DMatrix<bool, bool, Entry>(rowSize, 0);
        }

        HashMapArray<K, V>(const HashMapArray<K, V> &) = delete; // disable copy constructor
        HashMapArray<K, V> &operator=(const HashMapArray<K, V> &) = delete; // assign operator
        HashMapArray<K, V> &operator=(HashMapArray<K, V> &&) = delete; // replacement operator

        ~RDMatrix() {
            delete matrix;
            matrix = nullptr;
        }

//        bool operator==(const RDMatrix<R, T> &other) const {
//            if (other.getRowSize() != rowSize) {
//                return false;
//            }
//            for (size_t r = 0; r < rowSize; ++r) {
//                if (other[r].pointSize != matrix[r].pointSize) {
//                    return false;
//                }
//                // проверка объекта с произвольными данными
//                if (other[r].data != matrix[r].data) {
//                    return false;
//                }
//                for (size_t c = 0; c < matrix[r].pointSize; ++c) {
//                    if (matrix[r].points[c] != other[r][c]) {
//                        return false;
//                    } // compare each element
//                }
//            }
//            return true;
//        }
//
//        bool operator!=(const RDMatrix<R, T> &other) const {
//            return !((*this) == other);
//        }
//
//        bool equalsWithError(const RDMatrix<R, T> &other, const double &error, bool skipCompareData = false) const {
//            if (other.getRowSize() != rowSize) {
//                return false;
//            }
//            for (size_t r = 0; r < rowSize; ++r) {
//                if (other[r].pointSize != matrix[r].pointSize) {
//                    return false;
//                }
//                if (!skipCompareData && (matrix[r].data != other[r].data)) {
//                    return false;
//                }
//                for (size_t c = 0; c < matrix[r].pointSize; ++c) {
//                    const T &a = (*this)[r][c];
//                    const T &b = other[r][c];
//
//                    // compare each element as range b-error <= a <= b+error
//                    if (!(((b - error) <= a) && (a <= (b + error)))) {
////                        std::cout<< "matrix element [" << r << "x" << c << "] this " << a << " != " << b << std::endl;
//                        return false;
//                    }
//                }
//            }
//            return true;
//        }

        utils::R3DMatrix<bool, bool, Entry> *getMatrix() const {
            return matrix;
        }

        void pushValue(size_t rowIndex, const K &key, const V &value) {
            size_t colIndex = hashEngine->hashCode(key);
            Entry entry = {key, value};


//            matrix->writePoints();

            if (rowIndex < matrix->getRowSize()) {
                if (colIndex < (*matrix)[rowIndex].cellSize) {
                    // пройти по точкам с одинаковым хэшкодом (значения коллизий)
                    Cell& cell = (*matrix)[rowIndex][colIndex];
                    for (size_t i = 0; i<cell.pointSize; ++i) {
                        if (cell[i] == key) {
                            // найдено значение с уже имеющимся ключом
                        }
                    }
                } else {
                    // re-initialize cols memory
                    matrix->writePoints(rowIndex, colIndex, &entry, 1);
//                    initializePointsMemory(matrix[rowIndex], value.pointSize);
//                    matrix[rowIndex].pointSize = value.pointSize;
                }
            } else {
                // re-initialize row memory
                matrix->writePoints(rowIndex, colIndex, &entry, 1);

            }
        }


    private:
        utils::R3DMatrix<bool, bool, Entry>* matrix;
        hash::HashEngine<K>* hashEngine;
        size_t tableSize; // размер hash table


    };
}


#endif
