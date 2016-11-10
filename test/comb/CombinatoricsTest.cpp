#include "CombinatoricsTest.h"

namespace test {
	namespace combinatorics {

		void test_subset_backtrack_search() {
			comb::SubsetBacktrackSearch sb(2);

			bool arrA[] = { false, false, false, false, false, false, false, false, false, false };
			size_t inputN = 3;
			size_t k = 0;
			sb.backtrack(arrA, k, inputN);
		}

		void test_permutation_backtrack_search() {
			comb::PermutationBacktrackSearch sb(80);

			size_t arrA[] = { 0, 0, 0, 0, 0 };
			size_t inputN = 4;
			size_t k = 0;
			sb.backtrack(arrA, k, inputN);
		}

		void test_generate_symbols() {
			size_t totalLen = 800;
			size_t lineLen = 80;
			for (size_t i = 0; i < totalLen; i++) {
				char symbols[] = {'q', 'w', 'e', 'r', 'y', 'i', 'o', 'p',
				'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'
				};
			}
		}

		void combinatorics_test() {
			suite("SubsetBacktrackSearch");
			mytest(subset_backtrack_search);

			suite("PermutationBacktrackSearch");
//			mytest(permutation_backtrack_search);
		}

	}
}

