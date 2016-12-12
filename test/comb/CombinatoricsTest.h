#ifndef TEST_COMB_COMBINATORICSTEST_H
#define TEST_COMB_COMBINATORICSTEST_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <regex.h>

#include <cmath>
#include <random>
#include <chrono>         // std::chrono::seconds

#include "test.h"

#include "comb/SubsetBacktrackSearch.h"
#include "comb/PermutationBacktrackSearch.h"


namespace test {
    namespace combinatorics {

		struct SplitterInfo {
			bool isNewLine;
			bool isNewWord;
			size_t wordCharIndex;
			size_t lineCharIndex;
		};

        void combinatorics_test();
    }
}

#endif

