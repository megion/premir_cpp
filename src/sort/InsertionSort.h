#ifndef SRC_SORT_HEAPSORT_H
#define SRC_SORT_HEAPSORT_H

#include <stdio.h>
#include <iostream>

namespace sort {
	/**
	 * Insertion Sort (сортировка вставками)
	 */
	template<typename T>
	class InsertionSort {
	public:

		InsertionSort() {
		}

		void sort(T * const arr, size_t arrSize) {
			for (size_t i = 1; i < arrSize; ++i) {
				for (size_t j = i; ((j>0) && (arr[j]<arr[j-1])); j=j-1) {
					swap();
				}
			}
		}

	private:
		void swap() {

		}

	};
}

#endif
