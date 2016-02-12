#ifndef TEST_UTILS_CHASHSET_TEST_H
#define TEST_UTILS_CHASHSET_TEST_H

#include <iostream>
#include <chrono>         // std::chrono::seconds

#include "test.h"
#include "ml/LevensteinDistance.h"
#include "utils/CHashSet.h"
#include "utils/hash/StringHash.h"
#include "utils/hash/FloatHash.h"
#include "utils/hash/NumberHash.h"
#include "graphics/ChartThread.h"
#include "graphics/PointChart.h"

namespace test {
    namespace utils_cHashSet {

        void cHashSet_test();
    }
}

#endif

