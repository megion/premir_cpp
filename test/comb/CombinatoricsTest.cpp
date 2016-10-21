#include "CombinatoricsTest.h"

namespace test {
	namespace combinatorics {

		void test_subset_backtrack_search() {
			comb::SubsetBacktrackSearch sb(2);
//			bool arrA[100]; // = {1, 2, 2, 8, 9, 3, 4, 5, 3, 3, 6, 7};
			bool arrA[] = {false, false, false, false, false, false, false, false, false, false};
			size_t input = 3;
			std::cout << "back track start: " << std::endl;
			size_t k = 0;
			sb.backtrack(arrA, k, input);
		}

		void combinatorics_test() {
			suite("SubsetBacktrackSearch");
			mytest(subset_backtrack_search);
		}

	}
}

