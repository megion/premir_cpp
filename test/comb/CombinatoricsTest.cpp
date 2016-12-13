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

		void printWordSplitter(SplitterInfo& info) {
			if (info.wordCharIndex == (info.wordLen-1)) {
				std::cout << " "; // end of word 
				info.isNewWord = true;
				info.wordLen = info.newWordLen;
				info.wordCharIndex = 0;
			} else {
				info.isNewWord = false;
				info.wordCharIndex++;
			}
		}

		void printLineSplitter(SplitterInfo& info) {
			if (info.lineCharIndex == (info.lineLen-1)) {
				std::cout << std::endl; // end of line
				info.isNewLine = true;
				info.lineCharIndex = 0;
				info.wordCharIndex = 0;
				info.linesCount++;
			} else {
				info.isNewLine = false;
				info.lineCharIndex++;
			}
		}
		
		void printSplitters(SplitterInfo& info) {
			printLineSplitter(info);
			if (!info.isNewLine) {
				printWordSplitter(info);
			}
		}
		
		void test_generate_symbols() {
			//size_t totalLen = 800;
			size_t lineLen = 60;
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
			char symbols[] = {';', ','};
			size_t symbolsCount = 2;
			size_t upperLettersFreq = 6; // frequancy of appearance of the upper letter 
			size_t symbolsFreq = 4; // frequancy of appearance of the upper letter 

			std::random_device rd;     // only used once to initialise (seed) engine
			std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
			std::uniform_int_distribution<size_t> uni(0, lettersCount-1); // guaranteed unbiased
			std::uniform_int_distribution<size_t> upperFreqRandom(0, upperLettersFreq-1); // guaranteed unbiased
			std::uniform_int_distribution<size_t> symbolsFreqRandom(0, symbolsFreq-1); // 
			std::uniform_int_distribution<size_t> symbolsRandom(0, symbolsCount-1); // guaranteed unbiased
			std::uniform_int_distribution<size_t> wordLenRandom(4, 8); // guaranteed unbiased
			
			//std::default_random_engine generator;
			//std::normal_distribution<double> distribution(0.0, 0.1);
			SplitterInfo spInfo={false, false, 0, 0, wordLen, lineLen, wordLen, 0};
			//size_t charIndex = 0;
			size_t totalLines = 10;
			//size_t linesCount = 0;
			while (spInfo.linesCount < totalLines) {
				size_t lettersIndex = uni(rng);
   				if (upperFreqRandom(rng)==0) {
					size_t uLetterIndex = uni(rng);
					std::cout << upperLetters[uLetterIndex];
					spInfo.newWordLen = wordLenRandom(rng);
					printSplitters(spInfo); 
				}

				if (symbolsFreqRandom(rng)==0) {
					size_t symbolsIndex = symbolsRandom(rng);
					std::cout << symbols[symbolsIndex];
					spInfo.newWordLen = wordLenRandom(rng);
					printSplitters(spInfo); 
				}
				
				std::cout<<letters[lettersIndex];
				spInfo.newWordLen = wordLenRandom(rng);
				printSplitters(spInfo); 
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

