#ifndef SRC_COMB_SUBSET_BACKTRACK_SEARCH_H
#define SRC_COMB_SUBSET_BACKTRACK_SEARCH_H

#include <cstdio>
#include <iostream>

#include "BacktrackSearch.h"

namespace comb {
	/**
	 * Генерирование всех подмножеств для числа указанного в inputN
	 *
	 * generate all subset
	 *
	 * Чтобы сгенерировать все 2^n подмножеств,
	 * мы создаем массив (вектор) из n ячеек, в котором булево значение a[i], указывает,
	 * содержит ли данное подмножество i-й элемент.
	 *
	 * В данном случае у каждой вершины дерева будет по два дочерних узла (т.к. возможные кандидаты true,false)
	 * и глубина дерева будет равна 'k
	 */
	class SubsetBacktrackSearch: public BacktrackSearch<bool, size_t> {
	public:

		SubsetBacktrackSearch() :
				BacktrackSearch<bool, size_t>::BacktrackSearch() {
		}

		SubsetBacktrackSearch(size_t _maxCandidates) :
				BacktrackSearch<bool, size_t>::BacktrackSearch(_maxCandidates) {
		}

		bool isSolution(bool* const solutionsVector, size_t k, const size_t& inputN) {
			return k == inputN;
		}

		void processSolution(bool* const solutionsVector, size_t k, const size_t& inputN) {
			std::cout << "{";
			for (size_t i = 1; i <= k; i++) {
				if (solutionsVector[i] == true) {
					std::cout << " " << i;
				}
			}
			std::cout << " }" << std::endl;
		}

		void constractCandidates(bool* const solutionsVector, size_t k, bool* const candidates, size_t &numCandidates,
				const size_t& inputN) {
			numCandidates = 2;
			candidates[0] = true;
			candidates[1] = false;
		}

		void makeMove(bool* const solutionsVector, size_t k, const size_t& inputN) {
		}

		void unmakeMove(bool* const solutionsVector, size_t k, const size_t& inputN) {
		}

	};
}

#endif