#ifndef SRC_UTILS_CMATRIX_H_
#define SRC_UTILS_CMATRIX_H_

#include "CArrayList.h"

namespace utils {

/**
 * Реализация матрицы на основе CArrayList и следовательно предназначенной для
 * хранения объектов простых типов.
 */
    template<typename T>
    class CMatrix {
    public:

        CMatrix(size_t _rowSize, size_t _colSize) :
                rowSize(_rowSize), colSize(_colSize) {
            matrix = new CArrayList<T>(rowSize * colSize);
        }

        CMatrix() = delete;

        CMatrix(const CMatrix<T> &) = delete;

        CMatrix<T> &operator=(const CMatrix<T> &) = delete; // assign operator
        CMatrix<T> &operator=(CMatrix<T> &&) = delete; // replacement operator

        ~CMatrix() {
            delete matrix;
        }

        struct Coordinate {
            size_t r; // row
            size_t c; // col
        };

        // iterators
        class SliceIterator {
        public:
            SliceIterator(T *_arr, size_t _start, size_t _step, size_t _index) :
                    arr(_arr), start(_start), step(_step), pos(_start), //
                    index(_index) {
            }

            bool operator!=(const SliceIterator &other) const {
                return index != other.index;
            }

            T &operator*() const {
                return arr[pos];
            }

            const SliceIterator &operator++() {
                ++index;
                pos = start + index * step;
                return *this;
            }

        private:
            size_t start;
            size_t step;
            size_t index;
            size_t pos;
            T *arr;
        };

        class ColIterator {
        public:
            ColIterator(T *_arr, size_t _rsize, size_t _csize, size_t _index) :
                    arr(_arr), rsize(_rsize), csize(_csize), //
                    index(_index) {
            }

            SliceIterator begin() const {
                return SliceIterator(arr, index, csize, 0);
            }

            SliceIterator end() const {
                return SliceIterator(arr, 0, 0, rsize);
            }

        private:
            size_t rsize;
            size_t csize;
            size_t index;
            T *arr;
        };

        class RowIterator {
        public:
            RowIterator(T *_arr, size_t _rsize, size_t _csize, size_t _index) :
                    arr(_arr), rsize(_rsize), csize(_csize), //
                    index(_index) {
            }

            SliceIterator begin() const {
                return SliceIterator(arr, index * csize, 1, 0);
            }

            SliceIterator end() const {
                return SliceIterator(arr, 0, 0, csize);
            }

        private:
            size_t rsize;
            size_t csize;
            size_t index;
            T *arr;
        };
        ///////////////////////////////

        ColIterator col(size_t i) {
            return ColIterator(matrix->getArray(), rowSize, colSize, i);
        }

        RowIterator row(size_t i) {
            return RowIterator(matrix->getArray(), rowSize, colSize, i);
        }

        T &operator()(const size_t &r, const size_t &c) const {
            return (*matrix)[r * colSize + c];
        }

        bool operator==(const CMatrix<T> &other) const {
            if (other.getRowSize() != rowSize) {
                return false;
            }
            if (other.getColSize() != colSize) {
                return false;
            }
            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < colSize; ++c) {
                    if ((*this)(r, c) != other(r, c)) {
                        return false;
                    } // compare each element
                }
            }
            return true;
        }

        bool operator!=(const CMatrix<T> &other) const {
            return !((*this) == other);
        }

        bool equalsWithError(const CMatrix<T> &other, T error) const {
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

        Coordinate getCoordinate(size_t position) const {
            size_t r = position / colSize;
            size_t c = position - r * colSize;
            return {r, c};
        }

        CArrayList<T> *getMatrix() const {
            return matrix;
        }

        T *getRowArray(size_t r) const {
            return matrix->getArray() + r * colSize;
        }

        size_t getRowSize() const {
            return rowSize;
        }

        size_t getColSize() const {
            return colSize;
        }

        void writeRow(size_t rowIndex, const T *values) {
            size_t start = rowIndex * colSize;
            matrix->write(start, values, colSize);
        }

        void pushRow(const T *values) {
            writeRow(rowSize, values);
            ++rowSize;
        }

        void print() {
            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < colSize; ++c) {
                    std::cout << (*this)(r, c) << ", ";
                }
                std::cout << std::endl;
            }
        }

		/**
		 * Make matrix as identity (I)
		 */
		void makeIdentity() {
			for (size_t r = 0; r < rowSize; ++r) {
				for (size_t c = 0; c < colSize; ++c) {
					T& v = (*this)(r, c);
					if (r==c) { // main diagonal
						v = 1;
					} else {
						v = 0;
					}
				}
			}
		}

        CMatrix<T> *createClone(size_t startRow, size_t startCol,
                                size_t sizeRow, size_t sizeCol) const {
            CMatrix<T> *cop = new CMatrix<T>(sizeRow, sizeCol);
            for (size_t r = 0; r < sizeRow; ++r) {
                size_t start = (r + startRow) * colSize + startCol;
                cop->writeRow(r, (matrix->getArray()) + start);
            }
            return cop;
        }

        CMatrix<T> *createClone(size_t startRow, size_t startCol) const {
            return createClone(startRow, startCol, rowSize - startRow,
                               colSize - startCol);
        }

        CMatrix<T> *createClone() const {
            return createClone(0, 0, rowSize, colSize);
        }

        void swapRows(size_t r1, size_t r2) {
            T temp[colSize];
            size_t amountCol = colSize * matrix->getTypeSizeof();
            // copy r1 row to temp
            T* r1Arr = getRowArray(r1);
            memcpy(temp, r1Arr, amountCol);
            // copy r2 row to r1
            T* r2Arr = getRowArray(r2);
            memcpy(r1Arr, r2Arr, amountCol);
            // copy temp to r2
            memcpy(r2Arr, temp, amountCol);
        }

    private:
        CArrayList<T> *matrix;
        size_t rowSize;
        size_t colSize;
    };

}

#endif
