#ifndef SRC_COMB_BACKTRACK_SEARCH_H
#define SRC_COMB_BACKTRACK_SEARCH_H

#include <cstdio>

namespace comb {
	/**
	 * realization algorithm return search
	 * T - type of solution vector and candidates
	 * In - type of any common input data. For example specify size of a solution.
	 *
	 * Алгоритм перебора с возвратом
	 */
	template<typename T, typename In>
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
		 * k - deep of tree (initialize with 0)
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
		void backtrack(T* const solutionsVector, size_t k, const In& inputN) {
//			std::cout << "backtrack k = " << k << " ";
//			printSolutionsVector(solutionsVector, 10);
			if (isSolution(solutionsVector, k, inputN)) {
				processSolution(solutionsVector, k, inputN);
			} else {
				T candidates[maxCandidates]; // candidates for next position
				size_t numCandidates; // number of candidates for next position
				++k;
				constractCandidates(solutionsVector, k, candidates, numCandidates, inputN);
//				std::cout << "+++ start iterate candidates"  << std::endl;
				for (size_t i = 0; i < numCandidates; ++i) {
					solutionsVector[k] = candidates[i];
					makeMove(solutionsVector, k, inputN);
					backtrack(solutionsVector, k, inputN);
					unmakeMove(solutionsVector, k, inputN);
					if (finished) { // forced exit
						return;
					}
				}
//				std::cout << "--- end iterate candidates"  << std::endl;
			}
		}

		void printSolutionsVector(T* const solutionsVector, size_t vectorSize) {
			std::cout << "solutionsVector {";
			for (size_t i = 0; i < vectorSize; i++) {
				std::cout << " " << solutionsVector[i];
			}
			std::cout << " }" << std::endl;
		}

		/**
		 * return true if k elements of vector 'solutionsVector' are full solution
		 */
		virtual bool isSolution(T* const solutionsVector, size_t k, const In& inputN) = 0;

		/**
		 * process full solution
		 */
		virtual void processSolution(T* const solutionsVector, size_t k, const In& inputN) = 0;

		/**
		 * write full set off candidates to candidates array for 'k' position of vector 'solutionsVector'
		 */
		virtual void constractCandidates(T* const solutionsVector, size_t k, T* const candidates, size_t &numCandidates,
				const In& inputN) = 0;

		/**
		 * data modification
		 */
		virtual void makeMove(T* const solutionsVector, size_t k, const In& inputN) = 0;

		/**
		 * data modification
		 */
		virtual void unmakeMove(T* const solutionsVector, size_t k, const In& inputN) = 0;

	protected:
		bool finished; // all solutions have been found
		const static size_t MAX_CANDIDATES = 1000; // max count candidates for next position
		size_t maxCandidates;

	};
}

#endif