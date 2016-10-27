#ifndef SRC_COMB_PERMUTATION_BACKTRACK_SEARCH_H
#define SRC_COMB_PERMUTATION_BACKTRACK_SEARCH_H

#include <cstdio>
#include <iostream>

#include "BacktrackSearch.h"

namespace comb {
	/**
	 * generate all permutations for set {1,..., n} where n=inputN
	 */
	class PermutationBacktrackSearch: public BacktrackSearch<size_t, size_t> {
	public:

		PermutationBacktrackSearch() :
				BacktrackSearch<size_t, size_t>::BacktrackSearch() {
		}

		PermutationBacktrackSearch(size_t _maxCandidates) :
				BacktrackSearch<size_t, size_t>::BacktrackSearch(_maxCandidates) {
		}

		bool isSolution(size_t* const solutionsVector, size_t k, const size_t& inputN) {
			return k == inputN;
		}

		void processSolution(size_t* const solutionsVector, size_t k, const size_t& inputN) {
			std::cout << "{";
			for (size_t i = 1; i <= k; i++) {
				std::cout << " " << solutionsVector[i];
			}
			std::cout << " }" << std::endl;
		}

		void constractCandidates(size_t* const solutionsVector, size_t k, size_t* const candidates,
				size_t &numCandidates, const size_t& inputN) {
			bool inPerm[maxCandidates]; // what is element in permutation ?
			for (size_t i = 0; i < maxCandidates; i++) {
				inPerm[i] = false;
			}

			for (size_t i = 0; i < k; i++) {
//				std::cout << "k " << k << " i " << i <<" set to true inPerm[" << solutionsVector[i] << "]" << std::endl;
				inPerm[solutionsVector[i]] = true;
			}

			numCandidates = 0;
			for (size_t i = 1; i <= inputN; i++) {
				if (inPerm[i] == false) {
					candidates[numCandidates] = i;
					++numCandidates;
				}
			}
		}

		void makeMove(size_t* const solutionsVector, size_t k, const size_t& inputN) {
		}

		void unmakeMove(size_t* const solutionsVector, size_t k, const size_t& inputN) {
		}

	};
}

#endif
