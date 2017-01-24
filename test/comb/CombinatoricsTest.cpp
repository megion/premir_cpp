#include "CombinatoricsTest.h"

namespace test {
	namespace combinatorics {

		void test_subset_backtrack_search() {
			comb::SubsetBacktrackSearch sb(2);

			bool arrA[] = { false, false, false, false, false, false, false, false, false, false };
			size_t inputN = 3;
			size_t k = 0;
			utils::RDMatrix<bool, size_t> results; 
			sb.backtrack(arrA, k, inputN, &results);
			assert(results.getRowSize() == 8);
		}

		void test_permutation_backtrack_search() {
			comb::PermutationBacktrackSearch sb(80);

			size_t arrA[] = { 0, 0, 0, 0, 0 };
			size_t inputN = 4;
			size_t k = 0;
			utils::RDMatrix<bool, size_t> results; 
			sb.backtrack(arrA, k, inputN, &results);

			utils::hash::ArrayListSimpleTypeHash<size_t> listHashEngine;
			utils::HashSet<utils::CArrayList<size_t>> uniqueArrays(&listHashEngine);
			for (size_t i=0; i<results.getRowSize(); ++i) {
				utils::RDMatrix<bool, size_t>::Row& row = results[i];
				utils::CArrayList<size_t> list(row.points, row.pointSize);
				assert(uniqueArrays.contains(list) == false);
				uniqueArrays.push(list);
			}
		}

		void combinatorics_test() {
			suite("SubsetBacktrackSearch");
			mytest(subset_backtrack_search);

			suite("PermutationBacktrackSearch");
			mytest(permutation_backtrack_search);
		}

	}
}

