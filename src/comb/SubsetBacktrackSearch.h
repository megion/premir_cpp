#ifndef SRC_COMB_SUBSET_BACKTRACK_SEARCH_H
#define SRC_COMB_SUBSET_BACKTRACK_SEARCH_H

#include <cstdio>

#include "BacktrackSearch.h"

namespace comb {
	/**
	 * generate all subset
	 */
	class SubsetBacktrackSearch: public BacktrackSearch<bool, size_t> {
	public:

		SubsetBacktrackSearch() :
				BacktrackSearch<bool, size_t>::BacktrackSearch() {
		}

		bool isSolution(bool* const arr, size_t k, const size_t& input) {
			return k == input;
		}

		void processSolution(bool* const arr, size_t k, const size_t& input) {
		}

		void constractCandidates(bool* const arr, size_t k, bool* const candidates,
				const size_t &numCandidates, const size_t& input) {
		}

		void makeMove(bool* const arr, size_t k, const size_t& input) {
		}

		void unmakeMmove(bool* const arr, size_t k, const size_t& input) {
		}

	};
}

#endif
