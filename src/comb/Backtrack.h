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

		BacktrackSearch() : finished(false) {
		}

		void backtrack(T* const arr, size_t arrSize/*, data input*/) {
			if (isSolution(arr, arrSize)) {
				processSolution(arr, arrSize);
			} else {
				++arrSize;

			}
		}

	private:
		bool finished; // all solutions have been found

		/**
		 * return true if k elements of vector 'arr' are full solution
		 */
		bool isSolution(T* const arr, size_t k) {
			return false;
		}

		/**
		 * process full solution
		 */
		void processSolution(T* const arr, size_t k) {
		}

	};
}

#endif
