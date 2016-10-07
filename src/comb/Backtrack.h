#ifndef SRC_SORT_HEAPSORT_H
#define SRC_SORT_HEAPSORT_H

#include <stdio.h>
#include <iostream>

namespace sort {
	/**
	 * realization algorithm return search
	 */
	template<typename T>
	class BacktrackSearch {
	public:

		BacktrackSearch() {
		}

		void sort(T * const arr, size_t arrSize) {
			for (size_t i = 1; i < arrSize; ++i) {
				for (size_t j = i; ((j>0) && (arr[j]<arr[j-1])); j=j-1) {
//					swap();
				}
			}
		}

	private:
		bool finished;

	};
}

#endif
