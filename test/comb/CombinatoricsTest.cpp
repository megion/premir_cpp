#include "CombinatoricsTest.h"

namespace test {
	namespace combinatorics {

		void test_subset_backtrack_search() {
			comb::SubsetBacktrackSearch sb;
			bool arrA[100]; // = {1, 2, 2, 8, 9, 3, 4, 5, 3, 3, 6, 7};
			size_t input = 3;
			std::cout << "back track start: " << std::endl;
			sb.backtrack(arrA, 0, input);
		}

		void combinatorics_test() {
			suite("SubsetBacktrackSearch");
			mytest(subset_backtrack_search);
		}

	}
}

