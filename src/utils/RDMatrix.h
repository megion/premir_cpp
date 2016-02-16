/**
 * Simple matrix for store values as Row* with different col size
 */

#ifndef SRC_UTILS_RDMATRIX_H
#define SRC_UTILS_RDMATRIX_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

namespace utils {

    template<typename S, typename T>
    class RDMatrix {
    public:

        struct Row {
            size_t colSize;
            S data; // произвольные данные строки
            T *points; // массив однотипных данных строки
        };

        RDMatrix() : rowSize(0), matrix(nullptr),
                     tTypeSizeof(sizeof(T)), rTypeSizeof(sizeof(Row)),
                     sTypeSizeof(sizeof(S)) {
        }

        RDMatrix<S, T>(
                const RDMatrix<S, T> &) = delete; // disable copy constructor
        RDMatrix<S, T> &operator=(
                const RDMatrix<S, T> &) = delete; // assign operator
        RDMatrix<S, T> &operator=(
                RDMatrix<S, T> &&) = delete; // replacement operator

        ~RDMatrix() {
            removeAll();
        }

        void removeAll() {
            if (matrix) {
                // remove all row array
                for (size_t r = 0; r < rowSize; ++r) {
                    if (matrix[r].points) {
                        std::free(matrix[r].points);
                        // set to null each array pointer
                        matrix[r].points = nullptr;
                    }
                }
                std::free(matrix);
                matrix = nullptr;
                rowSize = 0;
            }
        }

        T &operator()(const size_t &r, const size_t &c) const {
            return matrix[r].points[c];
        }

        Row &operator[](const size_t &r) const {
            return matrix[r];
        }

        bool operator==(const RDMatrix<S, T> &other) const {
            if (other.getRowSize() != rowSize) {
                return false;
            }
            for (size_t r = 0; r < rowSize; ++r) {
                if (other.getRow(r).colSize != matrix[r].colSize) {
                    return false;
                }
                // проверка объекта с произвольными данными
                if (other.getRow(r).data != matrix[r].data) {
                    return false;
                }
                for (size_t c = 0; c < matrix[r].colSize; ++c) {
                    if (matrix[r].points[c] != other(r, c)) {
                        return false;
                    } // compare each element
                }
            }
            return true;
        }

        bool operator!=(const RDMatrix<S, T> &other) const {
            return !((*this) == other);
        }

        bool equalsWithError(const RDMatrix<S, T> &other,
                             const double &error,
                             bool skipCompareData = false) const {
            if (other.getRowSize() != rowSize) {
                return false;
            }
            for (size_t r = 0; r < rowSize; ++r) {
                if (other.getRow(r).colSize != matrix[r].colSize) {
                    return false;
                }

                if (!skipCompareData &&
                    (matrix[r].data != other.getRow(r).data)) {
                    return false;
                }
                for (size_t c = 0; c < matrix[r].colSize; ++c) {
                    const T &a = (*this)(r, c);
                    const T &b = other(r, c);

                    // compare each element as range b-error <= a <= b+error
                    if (!(((b - error) <= a) && (a <= (b + error)))) {
//                        std::cout<< "matrix element [" << r << "x" << c << "] this " << a << " != " << b << std::endl;
                        return false;
                    }
                }
            }
            return true;
        }

        size_t getRowSize() const {
            return rowSize;
        }

        Row *getMatrix() const {
            return matrix;
        }

        Row &getRow(const size_t &r) const {
            return matrix[r];
        }

        T *getRowPoints(const size_t &r) const {
            return matrix[r].points;
        }

        void writeRow(size_t rowIndex, const Row &value) {
            if (rowIndex < rowSize) {
                if (matrix[rowIndex].colSize < value.colSize) {
                    // re-initialize col memory
                    size_t cAmount = tTypeSizeof * value.colSize;
                    T *newPointsArray;
                    if (matrix[rowIndex].points) {
                        newPointsArray = (T *) std::realloc(matrix[rowIndex].points, cAmount);
                    } else {
                        newPointsArray = (T *) std::malloc(cAmount);
                    }

                    if (newPointsArray == NULL) {
                        throw std::runtime_error(std::strerror(errno));
                    }
                    matrix[rowIndex].points = newPointsArray;
                }
            } else {
                // re-initialize row memory
                size_t newRowSize = rowIndex + 1;
                initializeRowMemory(newRowSize);

                // initialize colSize = 0
                for (size_t r = rowSize; r < newRowSize; ++r) {
                    matrix[r].colSize = 0;
                    matrix[r].points = nullptr;
                }

                size_t cAmount = tTypeSizeof * value.colSize;
                T *pointsArray = (T *) std::malloc(cAmount);
                if (pointsArray == NULL) {
                    throw std::runtime_error(std::strerror(errno));
                }

                matrix[rowIndex].points = pointsArray;
                rowSize = newRowSize;
            }

            copyRow(matrix[rowIndex], value);
        }

        void writeRow(size_t rowIndex, T *points, size_t colSize) {
            Row value;
            value.points = points;
            value.colSize = colSize;
            writeRow(rowIndex, value);
        }

        void pushRow(const Row &value) {
            writeRow(rowSize, value);
        }

        void pushRow(T *points, size_t colSize) {
            Row value;
            value.points = points;
            value.colSize = colSize;
            pushRow(value);
        }

        void appendValues(size_t rowIndex, T *arr, size_t arrSize) {
            if (rowIndex < rowSize) {
                // добавим значения в конец существующей строки
                size_t oldColSize = matrix[rowIndex].colSize;
                size_t newColSize = oldColSize + arrSize;

                // re-initialize col memory
                size_t cAmount = tTypeSizeof * newColSize;
                T *newPointsArray;
                if (matrix[rowIndex].points) {
                    newPointsArray = (T *) std::realloc(matrix[rowIndex].points, cAmount);
                } else {
                    newPointsArray = (T *) std::malloc(cAmount);
                }

                if (newPointsArray == NULL) {
                    throw std::runtime_error(std::strerror(errno));
                }
                matrix[rowIndex].points = newPointsArray;
                matrix[rowIndex].colSize = newColSize;

                memcpy(matrix[rowIndex].points + oldColSize, arr, arrSize * tTypeSizeof);
            } else {
                // добавить новую строку
                writeRow(rowIndex, arr, arrSize);
            }
        }

        void appendValue(size_t rowIndex, T& val) {
            appendValues(rowIndex, &val, 1);
        }

        void print() const {
            std::cout << "RDMatrix[" << rowSize << "]" << std::endl;
            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < matrix[r].colSize; ++c) {
                    std::cout << matrix[r].points[c] << ", ";
                }
                std::cout << std::endl;
            }
        }

    private:
        Row *matrix;
        size_t rowSize;
        size_t tTypeSizeof; // saved value sizeof T
        size_t sTypeSizeof; // saved value sizeof S
        size_t rTypeSizeof; // saved value sizeof Row

        void initializeRowMemory(size_t _rowSize) {
            size_t rAmount = rTypeSizeof * _rowSize;
            Row *newMatrix;
            if (matrix) {
                newMatrix = (Row *) std::realloc(matrix, rAmount);
            } else {
                newMatrix = (Row *) std::malloc(rAmount);
            }

            if (newMatrix == NULL) {
                throw std::runtime_error(std::strerror(errno));
            }
            matrix = newMatrix;
        }

        void copyRow(Row &dest, const Row &src) {
            dest.colSize = src.colSize;
            // copy data
            memcpy(&(dest.data), &(src.data), sTypeSizeof);
            // copy points array
            memcpy(dest.points, src.points, dest.colSize * tTypeSizeof);
        }
    };
}


#endif
