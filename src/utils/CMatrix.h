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
//	// begin method range-based for loop
//	Iterator begin() const {
//		return Iterator(array, 0);
//	}
//	// begin method range-based for loop
//	Iterator end() const {
//		return Iterator(array, length);
//	}

	///////////////////////////////

private:
	CArrayList<T>* matrix;
	size_t rowSize;
	size_t columnSize;

};

}

#endif /* SRC_UTILS_CMATRIX_H_ */
