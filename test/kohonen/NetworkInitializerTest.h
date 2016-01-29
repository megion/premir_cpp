#ifndef TEST_KOHONEN_NETWORKINITIALIZER_TEST_H
#define TEST_KOHONEN_NETWORKINITIALIZER_TEST_H

#include <iostream>

#include "test.h"
#include "kohonen/NetworkInitializer.h"
#include "file/CsvFileReader.h"
#include "utils/SMatrix.h"
#include "kohonen/ArrayStreamReader.h"
#include "kohonen/RandomGenerator.h"
//#include "kohonen/winner/WinnerSearch.h"
#include "kohonen/winner/EuclideanWinnerSearch.h"
#include "kohonen/SomTrainer.h"

namespace test {
    namespace kohonen_initializer {
        void network_initializer_test();
    }
}

#endif

