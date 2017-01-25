#ifndef SRC_COMB_SUBSET_BACKTRACK_SEARCH_H
#define SRC_COMB_SUBSET_BACKTRACK_SEARCH_H

#include <cstdio>
#include <iostream>

#include "BacktrackSearch.h"

namespace comb {
	/**
	 * Генерирование всех подмножеств для числа указанного в solutionSize
	 *
	 * generate all subset
	 *
	 * Чтобы сгенерировать все 2^n подмножеств,
	 * мы создаем массив (вектор) из n ячеек, в котором булево значение a[i], указывает,
	 * содержит ли данное подмножество i-й элемент.
	 *
	 * В данном случае у каждой вершины дерева будет по два дочерних узла (т.к. возможные кандидаты true,false)
	 * и глубина дерева будет равна 'treeDepth'
	 */
	class SubsetBacktrackSearch: public BacktrackSearch<bool> {
	public:

		SubsetBacktrackSearch() :
				BacktrackSearch<bool>::BacktrackSearch() {
		}

		SubsetBacktrackSearch(size_t _maxCandidates) :
				BacktrackSearch<bool>::BacktrackSearch(_maxCandidates) {
		}

		bool isSolution(bool* const solutionsVector, size_t treeDepth, const size_t& solutionSize) {
			return treeDepth == solutionSize;
		}

		void processSolution(bool* const solutionsVector, size_t treeDepth, const size_t& solutionSize,
				utils::RDMatrix<bool, size_t>* solutionResults) {
			//std::cout << "{";
			size_t resultLen = 0;
			size_t resultVec[treeDepth]; // vector result set max size = tree depth
			for (size_t i = 0; i < treeDepth; i++) {
				if (solutionsVector[i] == true) {
					//std::cout << " " << i;
					resultVec[resultLen] = i;
					resultLen++;
				}
			}
			solutionResults->pushRow(resultVec, resultLen);
			//std::cout << " }" << std::endl;
		}

		void constractCandidates(bool* const solutionsVector, size_t treeDepth, bool* const candidates, size_t &numCandidates,
				const size_t& solutionSize) {
			numCandidates = 2;
			candidates[0] = true;
			candidates[1] = false;
		}

		void makeMove(bool* const solutionsVector, size_t treeDepth, const size_t& solutionSize) {
		}

		void unmakeMove(bool* const solutionsVector, size_t treeDepth, const size_t& solutionSize) {
		}

	};
}

#endif
