/**
 * Simple 3d matrix for store values as Row* with different col size
 */

#ifndef SRC_UTILS_R3DMATRIX_H
#define SRC_UTILS_R3DMATRIX_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

namespace utils {

    template<typename R, typename C, typename T>
    class R3DMatrix {
    public:

        struct Cell {
            size_t pointSize;
            C data; // произвольные данные ячейки
            T *points; // массив однотипных данных ячейки

            T &operator[](const size_t &p) const {
                return points[p];
            }
        };

        struct Row {
            size_t cellSize;
            R data; // произвольные данные строки
            Cell *cells; // массив ячеек строки

            Cell &operator[](const size_t &c) const {
                return cells[c];
            }
        };

        R3DMatrix() : rowSize(0), matrix(nullptr),
                      rTypeSizeof(sizeof(R)), cTypeSizeof(sizeof(C)), tTypeSizeof(sizeof(T)),
                      rowTypeSizeof(sizeof(Row)), cellTypeSizeof(sizeof(Cell)) {
        }

        R3DMatrix(size_t _rowSize, size_t _colSize) : rowSize(0), matrix(nullptr),
                                                      rTypeSizeof(sizeof(R)), cTypeSizeof(sizeof(C)),
                                                      tTypeSizeof(sizeof(T)),
                                                      rowTypeSizeof(sizeof(Row)), cellTypeSizeof(sizeof(Cell)) {
            initializeRowMemory(_rowSize);
            if (_colSize > 0) {
                for (size_t r = 0; r < rowSize; ++r) {
                    initializeCellsMemory(matrix[r], _colSize);
                }
            }
        }

        R3DMatrix<R, C, T>(const R3DMatrix<R, C, T> &) = delete; // disable copy constructor
        R3DMatrix<R, C, T> &operator=(const R3DMatrix<R, C, T> &) = delete; // assign operator
        R3DMatrix<R, C, T> &operator=(R3DMatrix<R, C, T> &&) = delete; // replacement operator

        ~R3DMatrix() {
            removeAll();
        }

        void removeAll() {
            if (matrix) {
                // remove all row array
                removeAllRowCells();
                std::free(matrix);
                matrix = nullptr;
                rowSize = 0;
            }
        }

        /**
         * remove all points and all cells. Rows not removed.
         */
        void removeAllRowCells() {
            for (size_t r = 0; r < rowSize; ++r) {
                // remove all points array for each cell
                removeRowPoints(r);
                if (matrix[r].cells) {
                    std::free(matrix[r].cells);
                    // set to null each array pointer
                    matrix[r].cells = nullptr;
                    matrix[r].cellSize = 0;
                }
            }
        }

        /**
         * remove all points array for each cells specified row.
         * Cells array for specified row not removed.
         */
        void removeRowPoints(const size_t &r) {
            for (size_t c = 0; c < matrix[r].cellSize; ++c) {
                if (matrix[r].cells[c].points) {
                    std::free(matrix[r].cells[c].points);
                    // set to null each array pointer
                    matrix[r].cells[c].points = nullptr;
                    matrix[r].cells[c].pointSize = 0;
                }
            }
        }

        T &operator()(const size_t &r, const size_t &c, const size_t &p) const {
            return matrix[r].cells[c].points[p];
        }

        Row &operator[](const size_t &r) const {
            return matrix[r];
        }

        bool operator==(const R3DMatrix<R, C, T> &other) const {
            if (other.getRowSize() != rowSize) {
                return false;
            }
            for (size_t r = 0; r < rowSize; ++r) {
                if (other.getRow(r).cellSize != matrix[r].cellSize) {
                    return false;
                }
                // проверка объекта с произвольными данными
                if (other.getRow(r).data != matrix[r].data) {
                    return false;
                }
                for (size_t c = 0; c < matrix[r].cellSize; ++c) {
                    if (matrix[r].cells[c].pointSize != other[r].cells[c].pointSize) {
                        return false;
                    }

                    if (matrix[r].cells[c].data != other[r].cells[c].data) {
                        return false;
                    }

                    for (size_t p = 0; p < matrix[r].cells[c].pointSize; ++p) {
                        if (matrix[r].cells[c].points[p] != other[r].cells[c].points[p]) {
                            return false;
                        }
                    }

                }
            }
            return true;
        }

        bool operator!=(const R3DMatrix<R, C, T> &other) const {
            return !((*this) == other);
        }

        bool equalsWithError(const R3DMatrix<R, C, T> &other,
                             const double &error,
                             bool skipCompareData = false) const {
            if (other.getRowSize() != rowSize) {
                return false;
            }
            for (size_t r = 0; r < rowSize; ++r) {
                if (other.getRow(r).cellSize != matrix[r].cellSize) {
                    return false;
                }
                // проверка объекта с произвольными данными
                if (!skipCompareData && other.getRow(r).data != matrix[r].data) {
                    return false;
                }
                for (size_t c = 0; c < matrix[r].cellSize; ++c) {
                    if (matrix[r].cells[c].pointSize != other[r].cells[c].pointSize) {
                        return false;
                    }

                    if (!skipCompareData && matrix[r].cells[c].data != other[r].cells[c].data) {
                        return false;
                    }

                    for (size_t p = 0; p < matrix[r].cells[c].pointSize; ++p) {
                        const T &a = matrix[r].cells[c].points[p];
                        const T &b = other[r].cells[c].points[p];
                        // compare each element as range b-error <= a <= b+error
                        if (!(((b - error) <= a) && (a <= (b + error)))) {
                            return false;
                        }
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

        void writePoints(size_t rowIndex, size_t colIndex, T *points, size_t pointSize, bool toEnd = false) {
            if (rowIndex < rowSize) {
                if (colIndex < matrix[rowIndex].cellSize) {
                    Cell &cell = matrix[rowIndex].cells[colIndex];
                    if (toEnd) {
                        // запись точек в конец
                        // добавим значения в конец существующей строки
                        size_t oldPointSize = cell.pointSize;
                        size_t newPointSize = oldPointSize + pointSize;

                        // re-initialize points memory
                        initializePointsMemory(cell, newPointSize);

                        std::memcpy(cell.points + oldPointSize, points, pointSize * tTypeSizeof);
                    } else {
                        // запись точек в начало
                        if (cell.pointSize < pointSize) {
                            initializePointsMemory(cell, pointSize);
                        } else {
                            // TODO: в случае если текущий размер точек больше или равен новому то размер ни как не меняем
                            // и точки просто будут записаны в начало массива.
                        }

                        // copy points array (dest, src, size)
                        std::memcpy(matrix[rowIndex].cells[colIndex].points, points, pointSize * tTypeSizeof);
                    }
                } else {
                    // re-initialize cells and points
                    size_t cellSize = colIndex + 1;
                    initializeCellsMemory(matrix[rowIndex], cellSize);

                    initializePointsMemory(matrix[rowIndex].cells[colIndex], pointSize);

                    // copy points array (dest, src, size)
                    std::memcpy(matrix[rowIndex].cells[colIndex].points, points, pointSize * tTypeSizeof);
                }
            } else {
                // re-initialize row memory
                size_t newRowSize = rowIndex + 1;
                initializeRowMemory(newRowSize);

                size_t cellSize = colIndex + 1;
                initializeCellsMemory(matrix[rowIndex], cellSize);

                initializePointsMemory(matrix[rowIndex].cells[colIndex], pointSize);

                // copy points array (dest, src, size)
                std::memcpy(matrix[rowIndex].cells[colIndex].points, points, pointSize * tTypeSizeof);
            }

        }

        void print(bool printEmpty = true) const {
            std::cout << "R3DMatrix[" << rowSize << "]" << std::endl;
            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < matrix[r].cellSize; ++c) {
                    if (printEmpty || matrix[r].cells[c].pointSize > 0) {
                        std::cout << "[" << r << "," << c << "]: ";
                        for (size_t p = 0; p < matrix[r].cells[c].pointSize; ++p) {
                            std::cout << matrix[r].cells[c].points[p] << ", ";
                        }
                        std::cout << std::endl;
                    }
                }

            }
        }

    private:
        Row *matrix;
        size_t rowSize;

        size_t rTypeSizeof; // saved value sizeof R
        size_t cTypeSizeof; // saved value sizeof C
        size_t tTypeSizeof; // saved value sizeof T

        size_t rowTypeSizeof; // saved value sizeof Row
        size_t cellTypeSizeof; // saved value sizeof Cell

        void initializeRowMemory(size_t newRowSize) {
            size_t rowAmount = rowTypeSizeof * newRowSize;
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

            // initialize cells = 0
            for (size_t r = rowSize; r < newRowSize; ++r) {
                matrix[r].cellSize = 0;
                matrix[r].cells = nullptr;
            }
            rowSize = newRowSize;
        }

        void initializeCellsMemory(Row &row, size_t cellSize) {
            size_t cellsAmount = cellTypeSizeof * cellSize;
            Cell *newCells;
            if (row.cells) {
                newCells = (Cell *) std::realloc(row.cells, cellsAmount);
            } else {
                newCells = (Cell *) std::malloc(cellsAmount);
            }

            if (newCells == NULL) {
                throw std::runtime_error(std::strerror(errno));
            }
            row.cells = newCells;

            // initialize cells.points = 0
            for (size_t c = row.cellSize; c < cellSize; ++c) {
                row.cells[c].pointSize = 0;
                row.cells[c].points = nullptr;
            }
            row.cellSize = cellSize;
        }

        void initializePointsMemory(Cell &cell, size_t pointsSize) {
            size_t pointsAmount = tTypeSizeof * pointsSize;
            T *newPoints;
            if (cell.points) {
                newPoints = (T *) std::realloc(cell.points, pointsAmount);
            } else {
                newPoints = (T *) std::malloc(pointsAmount);
            }

            if (newPoints == NULL) {
                throw std::runtime_error(std::strerror(errno));
            }
            cell.points = newPoints;
            cell.pointSize = pointsSize;
        }

    };
}


#endif
