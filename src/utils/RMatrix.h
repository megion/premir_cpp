/**
 * Simple matrix for store values as Row*
 */

#ifndef SRC_UTILS_RMATRIX_H
#define SRC_UTILS_RMATRIX_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

namespace utils {

    template<typename S, typename T>
    class RMatrix {
    public:

        struct Row {
            S data; // произвольные данные строки
            T* points; // массив однотипных данных строки
        };

        RMatrix(const size_t &_rowSize, const size_t &_colSize) :
                rowSize(_rowSize), colSize(_colSize), matrix(nullptr),
                tTypeSizeof(sizeof(T)), rTypeSizeof(sizeof(Row)),
                sTypeSizeof(sizeof(S)) {
            if (rowSize > 0) {
                initializeRowMemory(rowSize);
                size_t cAmount = tTypeSizeof * colSize;
                for (size_t r = 0; r < rowSize; ++r) {
                    T *pointsArray = (T *) std::malloc(cAmount);
                    if (pointsArray == NULL) {
                        throw std::runtime_error(std::strerror(errno));
                    }
                    matrix[r].points = pointsArray;
                }
            }
        }

        RMatrix() = delete; // disable default constructor
        RMatrix<S, T>(const RMatrix<S, T> &) = delete; // disable copy constructor
        RMatrix<S, T> &operator=(const RMatrix<S, T> &) = delete; // assign operator
        RMatrix<S, T> &operator=(RMatrix<S, T> &&) = delete; // replacement operator

        ~RMatrix() {
            if (matrix) {
                // remove all row array
                for (size_t r = 0; r < rowSize; ++r) {
                    std::free(matrix[r].points);
                    // set to null each array pointer
                    matrix[r].points = nullptr;
                }
                std::free(matrix);
                matrix = nullptr;
                rowSize = 0;
                colSize = 0;
            }
        }

        T &operator()(const size_t &r, const size_t &c) const {
            return matrix[r].points[c];
        }

        T* operator[](const size_t &r) const {
            return matrix[r].points;
        }

        bool operator==(const RMatrix<S, T> &other) const {
            if (other.getRowSize() != rowSize) {
                return false;
            }
            if (other.getColSize() != colSize) {
                return false;
            }
            for (size_t r = 0; r < rowSize; ++r) {
                // проверка объекта с произвольными данными
                if(matrix[r].data != other.getRow(r).data) {
                    return false;
                }
                for (size_t c = 0; c < colSize; ++c) {
                    if (matrix[r].points[c] != other(r, c)) {
                        return false;
                    } // compare each element
                }
            }
            return true;
        }

        bool operator!=(const RMatrix<S, T> &other) const {
            return !((*this) == other);
        }

        bool equalsWithError(const RMatrix<S, T> &other,
                             const double &error, bool skipCompareData=false) const {
            if (other.getRowSize() != rowSize) {
                return false;
            }
            if (other.getColSize() != colSize) {
                return false;
            }
            for (size_t r = 0; r < rowSize; ++r) {
                if(!skipCompareData && (matrix[r].data != other.getRow(r).data)) {
                    return false;
                }
                for (size_t c = 0; c < colSize; ++c) {
                    const T &a = (*this)(r, c);
                    const T &b = other(r, c);

                    // compare each element as range b-error <= a <= b+error
                    if (!(((b - error) <= a) && (a <= (b + error)))) {
                        std::cout<< "matrix element [" << r << "x" << c << "] this " << a << " != " << b << std::endl;
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

        Row* getMatrix() const {
            return matrix;
        }

        Row& getRow(const size_t &r) const {
            return matrix[r];
        }

        T* getRowPoints(const size_t &r) const {
            return matrix[r].points;
        }

        void writeRow(size_t rowIndex, T *points) {
            if (rowIndex < rowSize) {
                // copy only points to exist row
                memcpy(matrix[rowIndex].points, points, colSize * tTypeSizeof);
            } else {
                // create row value
                Row value;
                value.points = points;
                writeRow(rowIndex, value);
            }
        }

        void writeRow(size_t rowIndex, const Row& value) {
            if (!(rowIndex < rowSize)) {
                // re-initialize row memory
                size_t newRowSize = rowIndex + 1;
                initializeRowMemory(newRowSize);

                size_t cAmount = tTypeSizeof * colSize;
                for (size_t r = rowSize; r < newRowSize; ++r) {
                    T *pointsArray = (T *) std::malloc(cAmount);
                    if (pointsArray == NULL) {
                        throw std::runtime_error(std::strerror(errno));
                    }
                    matrix[r].points = pointsArray;
                }
                rowSize = newRowSize;
            }

            copyRow(matrix[rowIndex], value);
        }

        void pushRow(const Row& value) {
            writeRow(rowSize, value);
        }

        void pushRow(T *points) {
            writeRow(rowSize, points);
        }

        void swapRows(size_t r1, size_t r2) {
            // create temp row
            T tempPoints[colSize];
            Row tempRow;
            tempRow.points = tempPoints;

            // copy r1 row to temp
            copyRow(tempRow, matrix[r1]);
            // copy r2 row to r1
            copyRow(matrix[r1], matrix[r2]);
            // copy temp to r2
            copyRow(matrix[r2], tempRow);
        }

        RMatrix<S, T> *createClone() const {
            RMatrix<S, T> *clone = new RMatrix<S, T>(rowSize, colSize);
            for (size_t r = 0; r < rowSize; ++r) {
                clone->writeRow(r, matrix[r]);
            }
            return clone;
        }

        /**
         * Скопировать значения в матрицу destination
         */
        void copyTo(RMatrix<S, T>& destination) const {
            for (size_t r = 0; r < rowSize; ++r) {
                destination.writeRow(r, matrix[r]);
            }
        }

        void print() const {
            std::cout << "RMatrix["<<rowSize<<"x"<<colSize<<"]"<< std::endl;
            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < colSize; ++c) {
                    std::cout << matrix[r].points[c] << ", ";
                }
                std::cout << std::endl;
            }
        }

        void copyRow(Row& dest, const Row& src) {
            // copy data
            memcpy(&(dest.data), &(src.data), sTypeSizeof);
            // copy points array
            memcpy(dest.points, src.points, colSize * tTypeSizeof);
        }

        void copyRow(S& destData, T* destPoints, const Row& src) {
            // copy data
            memcpy(&(destData), &(src.data), sTypeSizeof);
            // copy points array
            memcpy(destPoints, src.points, colSize * tTypeSizeof);
        }

    private:
        Row *matrix;
        size_t rowSize;
        size_t colSize;
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
    };
}


#endif
