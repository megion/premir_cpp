#ifndef SRC_COMB_BACKTRACK_SEARCH_H
#define SRC_COMB_BACKTRACK_SEARCH_H

#include <cstdio>

namespace comb {
	/**
	 * realization algorithm return search
	 * In - type of any common input data. For example specify size of a solution.
	 */
	template<typename T, typename In>
	class BacktrackSearch {
	public:

		BacktrackSearch() :
				finished(false), maxCandidates(MAX_CANDIDATES) {
		}

		BacktrackSearch(size_t _maxCandidates) :
				finished(false), maxCandidates(_maxCandidates) {
		}

		void backtrack(T* const arr, size_t k, const In& input) {
			if (isSolution(arr, k, input)) {
				processSolution(arr, k, input);
			} else {
				T candidates[MAX_CANDIDATES]; // candidates for next position
				size_t numCandidates; // number of candidates for next position
				++k;
				constractCandidates(arr, k, candidates, numCandidates, input);
				for (size_t i = 0; i < numCandidates; ++i) {
					makeMove(arr, k, input);
					arr[i] = candidates[i];
					unmakeMove(arr, k, input);
					if (finished) {
						return;
					}
				}
			}
		}

		/**
		 * return true if k elements of vector 'arr' are full solution
		 */
		virtual bool isSolution(T* const arr, size_t k, const In& input) = 0;

		/**
		 * process full solution
		 */
		virtual void processSolution(T* const arr, size_t k, const In& input) = 0;

		/**
		 * write full set off candidates to candidates array for 'k' position of vector 'arr'
		 */
		virtual void constractCandidates(T* const arr, size_t k, T* const candidates,
				const size_t &numCandidates, const In& input) = 0;

		/**
		 * data modification
		 */
		virtual void makeMove(T* const arr, size_t k, const In& input) = 0;

		/**
		 * data modification
		 */
		virtual void unmakeMmove(T* const arr, size_t k, const In& input) = 0;

	protected:
		bool finished; // all solutions have been found
		const static size_t MAX_CANDIDATES = 1000; // max count candidates for next position
		size_t maxCandidates;

	};
}

#endif
