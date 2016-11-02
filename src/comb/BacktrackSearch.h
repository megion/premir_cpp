#ifndef SRC_COMB_BACKTRACK_SEARCH_H
#define SRC_COMB_BACKTRACK_SEARCH_H

#include <cstdio>

namespace comb {
	/**
	 * realization algorithm return search
	 * T - type of solution vector and candidates
	 *
	 * Алгоритм перебора с возвратом
	 */
	template<typename T>
	class BacktrackSearch {
	public:

		BacktrackSearch() :
				finished(false), maxCandidates(MAX_CANDIDATES) {
		}

		/**
		 * Max of candidates must be great or equals then k - deep of tree
		 */
		BacktrackSearch(size_t _maxCandidates) :
				finished(false), maxCandidates(_maxCandidates) {
		}

		/**
		 * solutionsVector - vector of solutions, 'arr[i]' store 'true' if current set contains element 'i'
		 * treeDepth - deep of tree (initialize with 0)
		 *
		 * На каждом этапе алгоритма перебора с возвратом мы пытаемся расширить данное
		 * частичное решение a = (a[1], a[2], ..., a[k]) добавляя следующий элемент в конец
		 * последовательности. После расширения последовательности нам нужно проверить, не содержит
		 * ли она полного решения.
		 *
		 * При переборе с возвратом создается дерево, в котором каждая вершина представляет
		 * частичное решение. Если узел Y создан в результате перехода от узла X, то эти узлы
		 * соединяются ребром. Такое дерево частичных решений предоставляет альтернативный
		 * взгляд на перебор с возвратом, т. к. процесс создания решений в точности
		 * соответствует процессу обхода в глубину дерева перебора с возвратом.
		 */
		void backtrack(T* const solutionsVector, size_t treeDepth, const size_t& solutionSize) {
			std::cout << "backtrack k = " << treeDepth << " ";
			printSolutionsVector(solutionsVector, solutionSize);
			if (isSolution(solutionsVector, treeDepth, solutionSize)) {
				processSolution(solutionsVector, treeDepth, solutionSize);
			} else {
				size_t currTreeDepth = treeDepth; // current tree depth
				++treeDepth; // increase depth
				T candidates[maxCandidates]; // candidates for next position
				size_t numCandidates; // number of candidates for next position
				constractCandidates(solutionsVector, currTreeDepth, candidates, numCandidates, solutionSize);
//				std::cout << "+++ start iterate candidates"  << std::endl;
				for (size_t i = 0; i < numCandidates; ++i) {
					solutionsVector[currTreeDepth] = candidates[i];
					makeMove(solutionsVector, currTreeDepth, solutionSize);
					backtrack(solutionsVector, treeDepth, solutionSize);
					unmakeMove(solutionsVector, currTreeDepth, solutionSize);
					if (finished) { // forced exit
						return;
					}
				}
//				std::cout << "--- end iterate candidates"  << std::endl;
			}
		}

		void printSolutionsVector(T* const solutionsVector, const size_t& solutionSize) {
			std::cout << "solutionsVector {";
			for (size_t i = 0; i < solutionSize; i++) {
				std::cout << " " << solutionsVector[i];
			}
			std::cout << " }" << std::endl;
		}

		/**
		 * return true if k elements of vector 'solutionsVector' are full solution
		 */
		virtual bool isSolution(T* const solutionsVector, size_t treeDepth, const size_t& solutionSize) = 0;

		/**
		 * process full solution
		 */
		virtual void processSolution(T* const solutionsVector, size_t treeDepth, const size_t& solutionSize) = 0;

		/**
		 * write full set off candidates to candidates array for 'treeDepth' position of vector 'solutionsVector'
		 */
		virtual void constractCandidates(T* const solutionsVector, size_t treeDepth, T* const candidates,
				size_t &numCandidates, const size_t& solutionSize) = 0;

		/**
		 * data modification
		 */
		virtual void makeMove(T* const solutionsVector, size_t treeDepth, const size_t& solutionSize) = 0;

		/**
		 * data modification
		 */
		virtual void unmakeMove(T* const solutionsVector, size_t treeDepth, const size_t& solutionSize) = 0;

	protected:
		bool finished; // all solutions have been found
		const static size_t MAX_CANDIDATES = 1000; // max count candidates for next position
		size_t maxCandidates;

	};
}

#endif
