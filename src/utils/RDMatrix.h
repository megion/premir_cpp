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

    template<typename R, typename T>
    class RDMatrix {
    public:

        struct Row {
            size_t pointSize;
            size_t pointCapacity;
            R data; // произвольные данные строки
            T *points; // массив однотипных данных строки

            T &operator[](const size_t &p) const {
                return points[p];
            }
        };

        RDMatrix() : rowSize(0), rowCapacity(rowSize), rowCapacityIncrease(1), pointCapacityIncrease(1),
                     matrix(nullptr), tTypeSizeof(sizeof(T)), rowTypeSizeof(sizeof(Row)), rTypeSizeof(sizeof(R)) {
        }

        RDMatrix(size_t _rowSize, size_t _rowCapacityIncrease, size_t _pointCapacityIncrease) :
                rowSize(0), rowCapacity(rowSize), rowCapacityIncrease(_rowCapacityIncrease),
                pointCapacityIncrease(_pointCapacityIncrease), matrix(nullptr),
                tTypeSizeof(sizeof(T)), rowTypeSizeof(sizeof(Row)), rTypeSizeof(sizeof(R)) {
            initializeRowMemory(_rowSize);
        }

        RDMatrix(size_t _rowSize, size_t _pointSize) :
                rowSize(0), rowCapacity(rowSize), rowCapacityIncrease(1), pointCapacityIncrease(1),
                matrix(nullptr), tTypeSizeof(sizeof(T)), rowTypeSizeof(sizeof(Row)), rTypeSizeof(sizeof(R)) {
            initializeRowMemory(_rowSize);
            for (size_t r=0; r<rowSize; ++r) {
                initializePointsMemory(matrix[r], _pointSize);
            }
        }

        RDMatrix<R, T>(const RDMatrix<R, T> &) = delete; // disable copy constructor
        RDMatrix<R, T> &operator=(const RDMatrix<R, T> &) = delete; // assign operator
        RDMatrix<R, T> &operator=(RDMatrix<R, T> &&) = delete; // replacement operator

        ~RDMatrix() {
            removeAll();
        }

        void setRowCapacityIncrease(size_t _rowCapacityIncrease) {
            rowCapacityIncrease = _rowCapacityIncrease;
        }

        void setPointCapacityIncrease(size_t _pointCapacityIncrease) {
            pointCapacityIncrease = _pointCapacityIncrease;
        }

        void removeAll() {
            if (matrix) {
                // remove all row array
                for (size_t r = 0; r < rowCapacity; ++r) {
                    if (matrix[r].points) {
                        std::free(matrix[r].points);
                        // set to null each array pointer
                        matrix[r].points = nullptr;
                        matrix[r].pointSize = 0;
                        matrix[r].pointCapacity = 0;
                    }
                }
                std::free(matrix);
                matrix = nullptr;
                rowSize = 0;
                rowCapacity = 0;
            }
        }

        T &operator()(const size_t &r, const size_t &c) const {
            return matrix[r].points[c];
        }

        Row &operator[](const size_t &r) const {
            return matrix[r];
        }

        bool operator==(const RDMatrix<R, T> &other) const {
            if (other.getRowSize() != rowSize) {
                return false;
            }
            for (size_t r = 0; r < rowSize; ++r) {
                if (other[r].pointSize != matrix[r].pointSize) {
                    return false;
                }
                // проверка объекта с произвольными данными
                if (other[r].data != matrix[r].data) {
                    return false;
                }
                for (size_t c = 0; c < matrix[r].pointSize; ++c) {
                    if (matrix[r].points[c] != other[r][c]) {
                        return false;
                    } // compare each element
                }
            }
            return true;
        }

        bool operator!=(const RDMatrix<R, T> &other) const {
            return !((*this) == other);
        }

        bool equalsWithError(const RDMatrix<R, T> &other, const double &error, bool skipCompareData = false) const {
            if (other.getRowSize() != rowSize) {
                return false;
            }
            for (size_t r = 0; r < rowSize; ++r) {
                if (other[r].pointSize != matrix[r].pointSize) {
                    return false;
                }
                if (!skipCompareData && (matrix[r].data != other[r].data)) {
                    return false;
                }
                for (size_t c = 0; c < matrix[r].pointSize; ++c) {
                    const T &a = (*this)[r][c];
                    const T &b = other[r][c];

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
            if (rowIndex < rowCapacity) {
                if (matrix[rowIndex].pointCapacity < value.pointSize) {
                    // re-initialize points memory
                    initializePointsMemory(matrix[rowIndex], value.pointSize);
                } else {
                    matrix[rowIndex].pointSize = value.pointSize;
                }
                if (rowIndex >= rowSize) {
                    rowSize = rowIndex + 1;
                }
            } else {
                // re-initialize row memory
                size_t newRowSize = rowIndex + 1;
                initializeRowMemory(newRowSize);

                initializePointsMemory(matrix[rowIndex], value.pointSize);
            }

            copyRow(matrix[rowIndex], value);
        }

        void writeRow(size_t rowIndex, T *points, size_t pointSize) {
            Row value;
            value.points = points;
            value.pointSize = pointSize;
            writeRow(rowIndex, value);
        }

        void pushRow(const Row &value) {
            writeRow(rowSize, value);
        }

        void pushRow(T *points, size_t pointSize) {
            Row value;
            value.points = points;
            value.pointSize = pointSize;
            pushRow(value);
        }

        /**
         * Записать значения в конец строки
         */
        void writeToEndRow(size_t rowIndex, T *arr, size_t arrSize) {
            if (rowIndex < rowSize) {
                // добавим значения в конец существующей строки
                size_t oldPointSize = matrix[rowIndex].pointSize;
                size_t newPointSize = oldPointSize + arrSize;

                // re-initialize points memory
                if (matrix[rowIndex].pointCapacity < newPointSize) {
                    initializePointsMemory(matrix[rowIndex], newPointSize);
                } else {
                    matrix[rowIndex].pointSize = newPointSize;
                }

                memcpy(matrix[rowIndex].points + oldPointSize, arr, arrSize * tTypeSizeof);
            } else {
                // добавить новую строку
                writeRow(rowIndex, arr, arrSize);
            }
        }

        void writeToEndRow(size_t rowIndex, T& val) {
            writeToEndRow(rowIndex, &val, 1);
        }

        void print() const {
            std::cout << "RDMatrix[" << rowSize << "]" << std::endl;
            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < matrix[r].pointSize; ++c) {
                    std::cout << matrix[r].points[c] << ", ";
                }
                std::cout << std::endl;
            }
        }

    private:
        Row *matrix;

        size_t rowSize; // число строк
        size_t rowCapacity; // число строк для которых выделена память
        size_t rowCapacityIncrease; // число на которое увеличивается rowCapacity при выделении памяти
        size_t pointCapacityIncrease; // число на которое увеличивается pointCapacity при выделении памяти

        size_t tTypeSizeof; // saved value sizeof T
        size_t rTypeSizeof; // saved value sizeof R
        size_t rowTypeSizeof; // saved value sizeof Row

        void initializeRowMemory(size_t newRowSize) {
            size_t newRowCapacity = 0;
            if (newRowSize > (rowCapacity + rowCapacityIncrease)) {
                newRowCapacity = newRowSize;
            } else {
                newRowCapacity = rowCapacity + rowCapacityIncrease;
            }

            size_t rowAmount = rowTypeSizeof * newRowCapacity;
            Row *newMatrix;
            if (matrix) {
                newMatrix = (Row *) std::realloc(matrix, rowAmount);
            } else {
                newMatrix = (Row *) std::malloc(rowAmount);
            }

            if (newMatrix == NULL) {
                throw std::runtime_error(std::strerror(errno));
            }
            matrix = newMatrix;

            // clean points
            for (size_t r = rowCapacity; r < newRowCapacity; ++r) {
                matrix[r].pointSize = 0;
                matrix[r].pointCapacity = 0;
                matrix[r].points = nullptr;
            }

            rowCapacity = newRowCapacity;
            rowSize = newRowSize;
        }

        void initializePointsMemory(Row& row, size_t newPointsSize) {
            size_t newPointsCapacity = 0;
            if (newPointsSize > (row.pointCapacity + pointCapacityIncrease)) {
                newPointsCapacity = newPointsSize;
            } else {
                newPointsCapacity = row.pointCapacity + pointCapacityIncrease;
            }

            size_t pointsAmount = tTypeSizeof * newPointsCapacity;
            T *newPoints;
            if (row.points) {
                newPoints = (T *) std::realloc(row.points, pointsAmount);
            } else {
                newPoints = (T *) std::malloc(pointsAmount);
            }

            if (newPoints == NULL) {
                throw std::runtime_error(std::strerror(errno));
            }
            row.points = newPoints;
            row.pointSize = newPointsSize;
            row.pointCapacity = newPointsCapacity;
        }

        void copyRow(Row &dest, const Row &src) {
//            dest.pointSize = src.pointSize;
            // copy data
            memcpy(&(dest.data), &(src.data), rTypeSizeof);
            // copy points array
            memcpy(dest.points, src.points, dest.pointSize * tTypeSizeof);
        }
    };
}


#endif
