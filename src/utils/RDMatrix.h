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

        RDMatrix() = delete; // disable default constructor
        RDMatrix<S, T>(
                const RDMatrix <S, T> &) = delete; // disable copy constructor
        RDMatrix <S, T> &operator=(
                const RDMatrix <S, T> &) = delete; // assign operator
        RDMatrix <S, T> &operator=(
                RDMatrix <S, T> &&) = delete; // replacement operator

        ~RDMatrix() {
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
            }
        }

        T &operator()(const size_t &r, const size_t &c) const {
            return matrix[r].points[c];
        }

        Row& operator[](const size_t &r) const {
            return matrix[r];
        }

        bool operator==(const RDMatrix <S, T> &other) const {
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

        bool operator!=(const RDMatrix <S, T> &other) const {
            return !((*this) == other);
        }

        bool equalsWithError(const RDMatrix <S, T> &other,
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

        void pushRow(const Row &value) {
            // re-initialize row memory
            size_t newRowSize = rowSize + 1;
            initializeRowMemory(newRowSize);

            size_t cAmount = tTypeSizeof * value.colSize;
            T *pointsArray = (T *) std::malloc(cAmount);
            if (pointsArray == NULL) {
                throw std::runtime_error(std::strerror(errno));
            }
            matrix[rowSize].points = pointsArray;
            copyRow(matrix[rowSize], value);
            rowSize = newRowSize;
        }

        void pushRow(const T *points, size_t colSize) {
            Row value;
            value.points = points;
            value.colSize = colSize;
            pushRow(value);
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
            memcpy(dest.points, src.points, colSize * tTypeSizeof);
        }
    };
}


#endif
