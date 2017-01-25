#ifndef SRC_COMB_PERMUTATION_BACKTRACK_SEARCH_H
#define SRC_COMB_PERMUTATION_BACKTRACK_SEARCH_H

#include <cstdio>
#include <iostream>

#include "BacktrackSearch.h"

namespace comb {
	/**
	 * generate all permutations for set {1,..., n} where n=solutionSize
	 */
	class PermutationBacktrackSearch: public BacktrackSearch<size_t> {
	public:

		PermutationBacktrackSearch() :
				BacktrackSearch<size_t>::BacktrackSearch() {
		}

		PermutationBacktrackSearch(size_t _maxCandidates) :
				BacktrackSearch<size_t>::BacktrackSearch(_maxCandidates) {
		}

		bool isSolution(size_t* const solutionsVector, size_t treeDepth, const size_t& solutionSize) {
			return treeDepth == (solutionSize-1);
		}

		void processSolution(size_t* const solutionsVector, size_t treeDepth, const size_t& solutionSize,
				utils::RDMatrix<bool, size_t>* solutionResults) {
			//std::cout << "{";
			size_t resultLen = 0;
			size_t resultVec[treeDepth]; // vector result set max size = tree depth
			for (size_t i = 0; i < treeDepth; i++) {
				//std::cout << " " << solutionsVector[i];
				resultVec[resultLen] = solutionsVector[i];
				resultLen++;
			}
			solutionResults->pushRow(resultVec, resultLen);
			//std::cout << " }" << std::endl;
		}

		void constractCandidates(size_t* const solutionsVector, size_t treeDepth, size_t* const candidates,
				size_t &numCandidates, const size_t& solutionSize) {
			bool inPerm[maxCandidates]; // what is element in permutation ?
			for (size_t i = 0; i < maxCandidates; i++) {
				inPerm[i] = false;
			}

			for (size_t i = 0; i < treeDepth; i++) {
//				std::cout << "k " << k << " i " << i <<" set to true inPerm[" << solutionsVector[i] << "]" << std::endl;
				inPerm[solutionsVector[i]] = true;
			}

			numCandidates = 0;
			for (size_t i = 0; i < solutionSize; i++) {
				if (inPerm[i] == false) {
					candidates[numCandidates] = i;
					++numCandidates;
				}
			}
		}

		void makeMove(size_t* const solutionsVector, size_t treeDepth, const size_t& solutionSize) {
		}

		void unmakeMove(size_t* const solutionsVector, size_t treeDepth, const size_t& solutionSize) {
		}

	};
}

#endif
