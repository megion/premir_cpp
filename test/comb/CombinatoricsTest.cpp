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
			size_t wordLen = 8;
			char letters[] = {'q', 'w', 'e', 'r', 't', 'y','u', 'i', 'o', 'p',
				'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
				'z', 'x', 'c', 'v', 'b', 'n', 'm'
			};
            char upperLetters[] = {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
				'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
				'Z', 'X', 'C', 'V', 'B', 'N', 'M'
			};
			size_t lettersCount = 26;
			char symbols[] = {';', ':'};
			size_t symbolsCount = 2;
			size_t upperLettersFreq = 6; // frequancy apper upper letter 

			std::random_device rd;     // only used once to initialise (seed) engine
			std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
			std::uniform_int_distribution<size_t> uni(0, lettersCount-1); // guaranteed unbiased
			std::uniform_int_distribution<size_t> upperFreqRandom(0, upperLettersFreq-1); // guaranteed unbiased
			
			//std::default_random_engine generator;
			//std::normal_distribution<double> distribution(0.0, 0.1);
			for (size_t i = 0; i < totalLen; i++) {
				size_t lettersIndex = uni(rng);
				size_t uLetterIndex = uni(rng);
				bool isNewLine = false;
				if ((i%lineLen == 0) && (i != 0)) {
					std::cout << std::endl; // end of line
					isNewLine = true;
				}
				if ((i%wordLen == 0) && (i != 0) && !isNewLine) {
					std::cout << " "; // end of word 
				}

				if (upperFreqRandom(rng)==0) {
					std::cout << upperLetters[uLetterIndex];
				}
				
				std::cout<<letters[lettersIndex];
			}
			std::cout << std::endl; // end of line
		}

		void combinatorics_test() {
			suite("SubsetBacktrackSearch");
			mytest(subset_backtrack_search);

			suite("PermutationBacktrackSearch");
//			mytest(permutation_backtrack_search);
//
			suite("Generate symbols");
			mytest(generate_symbols);
		}

	}
}

