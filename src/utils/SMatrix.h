/**
 * Simple matrix for store values as T**
 */

#ifndef SRC_UTILS_SMATRIX_H
#define SRC_UTILS_SMATRIX_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

namespace utils {

    template<typename T>
    class SMatrix {
    public:

        SMatrix(const size_t &_rowSize, const size_t &_colSize) :
                rowSize(_rowSize), colSize(_colSize), matrix(nullptr),
                typeSizeof(sizeof(T)), ptypeSizeof(sizeof(T *)) {
            if (rowSize > 0) {
                initializeRowMemory(rowSize);
                size_t cAmount = typeSizeof * colSize;
                for (size_t r = 0; r < rowSize; ++r) {
                    T *rowArray = (T *) std::malloc(cAmount);
                    if (rowArray == NULL) {
                        throw std::runtime_error(std::strerror(errno));
                    }
                    *(matrix + r) = rowArray;
                }
            }
        }

        SMatrix() = delete; // disable default constructor
        SMatrix<T>(const SMatrix<T> &) = delete; // disable copy constructor
        SMatrix<T> &operator=(const SMatrix<T> &) = delete; // assign operator
        SMatrix<T> &operator=(SMatrix<T> &&) = delete; // replacement operator

        ~SMatrix() {
            if (matrix) {
                // remove all row array
                for (size_t r = 0; r < rowSize; ++r) {
                    std::free(*(matrix + r));
                    *(matrix + r) = nullptr; // set to null each array pointer
                }
                std::free(matrix);
                matrix = nullptr;
                rowSize = 0;
                colSize = 0;
            }
        }

        T &operator()(const size_t &r, const size_t &c) const {
            return matrix[r][c];//*(*(matrix + r) + c);
        }

        T* operator[](const size_t &r) const {
            return matrix[r];
        }

        bool operator==(const SMatrix<T> &other) const {
            if (other.getRowSize() != rowSize) {
                return false;
            }
            if (other.getColSize() != colSize) {
                return false;
            }
            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < colSize; ++c) {
                    if (matrix[r][c] != other(r, c)) {
                        return false;
                    } // compare each element
                }
            }
            return true;
        }

        bool operator!=(const SMatrix<T> &other) const {
            return !((*this) == other);
        }

        bool equalsWithError(const SMatrix<T> &other,
                             const double &error) const {
            if (other.getRowSize() != rowSize) {
                return false;
            }
            if (other.getColSize() != colSize) {
                return false;
            }
            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < colSize; ++c) {
                    const T &a = (*this)(r, c);
                    const T &b = other(r, c);

                    // compare each element as range b-error <= a <= b+error
                    if (!(((b - error) <= a) && (a <= (b + error)))) {
                        return false;
                    }
                }
            }
            return true;
        }

        size_t getRowSize() const {
            return rowSize;
        }

        size_t getColSize() const {
            return colSize;
        }

        T **getMatrix() const {
            return matrix;
        }

        T *getRow(const size_t &r) const {
            return matrix[r];
        }

        void writeRow(size_t rowIndex, const T *values) {
            if (!(rowIndex < rowSize)) {
                // re-initialize row memory
                size_t newRowSize = rowIndex + 1;
                initializeRowMemory(newRowSize);


                size_t cAmount = typeSizeof * colSize;
                for (size_t r = rowSize; r < newRowSize; ++r) {
                    T *rowArray = (T *) std::malloc(cAmount);
                    if (rowArray == NULL) {
                        throw std::runtime_error(std::strerror(errno));
                    }
                    *(matrix + r) = rowArray;
                }
                rowSize = newRowSize;
            }
            memcpy(*(matrix + rowIndex), values, colSize * typeSizeof);
        }

        void pushRow(const T *values) {
            writeRow(rowSize, values);
        }

        void swapRows(size_t r1, size_t r2) {
            // copy r1 row to temp
            T *temp = *(matrix + r1);
            // copy r2 row to r1
            *(matrix + r1) = *(matrix + r2);
            // copy temp to r2
            *(matrix + r2) = temp;
        }

        SMatrix<T> *createClone(size_t startRow, size_t startCol,
                                size_t sizeRow, size_t sizeCol) const {
            SMatrix<T> *clone = new SMatrix<T>(sizeRow, sizeCol);
            for (size_t r = 0; r < sizeRow; ++r) {
                clone->writeRow(r, matrix[r + startRow] + startCol);
            }
            return clone;
        }

        SMatrix<T> *createClone(size_t startRow, size_t startCol) const {
            return createClone(startRow, startCol, rowSize - startRow,
                               colSize - startCol);
        }

        SMatrix<T> *createClone() const {
            return createClone(0, 0, rowSize, colSize);
        }

        void print() const {
            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < colSize; ++c) {
                    std::cout << matrix[r][c] << ", ";
                }
                std::cout << std::endl;
            }
        }

    private:
        T **matrix;
        size_t rowSize;
        size_t colSize;
        size_t typeSizeof; // saved value sizeof T
        size_t ptypeSizeof; // saved value sizeof T*

        void initializeRowMemory(size_t _rowSize) {
            size_t rAmount = ptypeSizeof * _rowSize;
            T **newMatrix;
            if (matrix) {
                newMatrix = (T **) std::realloc(matrix, rAmount);
            } else {
                newMatrix = (T **) std::malloc(rAmount);
            }

            if (newMatrix == NULL) {
                throw std::runtime_error(std::strerror(errno));
            }
            matrix = newMatrix;
        }
    };
}


#endif
