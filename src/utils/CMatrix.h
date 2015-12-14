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

	CMatrix(size_t _rowSize, size_t _columnSize) :
			rowSize(_rowSize), columnSize(_columnSize) {
		matrix = new CArrayList<T>(rowSize * columnSize);
	}

	~CMatrix() {
		delete matrix;
	}

	// iterator implementation for use in C++11 range-based for loops
	class SliceIterator {
	public:
		SliceIterator(T* _arr, size_t _start, size_t _step) :
				arr(_arr), start(_start), step(_step), pos(_start), counter(0) {
		}
		bool operator!=(const SliceIterator& other) const {
			return pos != other.pos;
		}
		T& operator*() const {
			return arr[pos];
		}
		const SliceIterator& operator++() {
			++counter;
			pos = start + counter * step;
			return *this;
		}

	private:
		size_t start;
		size_t step;
		size_t counter;
		size_t pos;
		T* arr;
	};

//	class RangeIterator {
//	public:
//		RangeIterator(T* _arr, size_t _dim1, size_t _dim2) :
//				arr(_arr), dim1(_dim1), dim2(_dim2) {
//		}
//		//	// begin method range-based for loop
//		SliceIterator begin() const {
//			return SliceIterator(arr, 0);
//		}
//		// begin method range-based for loop
//		SliceIterator end() const {
//			return SliceIterator(arr, length);
//		}
//
//	private:
//		size_t dim1;
//		size_t dim2;
//		T* arr;
//	};


///////////////////////////////

private:
	CArrayList<T>* matrix;
	size_t rowSize;
	size_t columnSize;

};

}

#endif /* SRC_UTILS_CMATRIX_H_ */
