#ifndef TEST_KOHONEN_NETWORKINITIALIZER_TEST_H
#define TEST_KOHONEN_NETWORKINITIALIZER_TEST_H

#include <iostream>
#include <cmath>

#include "test.h"
#include "kohonen/NetworkInitializer.h"
#include "file/CsvFileReader.h"
#include "utils/SMatrix.h"
#include "utils/RMatrix.h"
#include "file/stream/CsvFileArrayStreamReader.h"
#include "kohonen/RandomGenerator.h"
#include "kohonen/winner/EuclideanWinnerSearch.h"
#include "kohonen/alphafunc/InverseAlphaFunction.h"
#include "kohonen/alphafunc/LinearAlphaFunction.h"

#include "kohonen/neighadap/BubbleNeighborAdaptation.h"
#include "kohonen/neighadap/GaussianNeighborAdaptation.h"

#include "kohonen/mapdist/HexaMapDistance.h"
#include "kohonen/mapdist/RectMapDistance.h"

#include "kohonen/SomTrainer.h"

#include "graphics/ChartThread.h"

namespace test {
    namespace kohonen_initializer {
        void network_initializer_test();
    }
}

#endif

