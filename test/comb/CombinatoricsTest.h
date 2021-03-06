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
#include "utils/HashSet.h"
#include "utils/CArrayList.h"
#include "utils/hash/ArrayListHash.h"


namespace test {
    namespace combinatorics {

        void combinatorics_test();
    }
}

#endif

