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

		void backtrack(T* const arr, size_t k/*, data input*/) {
			if (isSolution(arr, k)) {
				processSolution(arr, k);
			} else {
				T candidates[MAX_CANDIDATES]; // candidates for next position
				size_t numCandidates; // number of candidates for next position
				++k;
				constractCandidates(arr, k, candidates, numCandidates);
				for (size_t i = 0; i < numCandidates; ++i) {
					arr[i] = candidates[i];
				}

			}
		}

	private:
		bool finished; // all solutions have been found
		const static size_t MAX_CANDIDATES = 1000; // max count candidates for next position

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


		/**
		 * write full set off candidates to candidates array for 'k' position of vector 'arr'
		 */
		void constractCandidates(T* const arr, size_t k, T* const candidates, const size_t &numCandidates) {
		}

	};
}

#endif
