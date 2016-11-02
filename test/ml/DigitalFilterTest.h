#ifndef TEST_ML_DIGITAL_FILTER_TEST_H
#define TEST_ML_DIGITAL_FILTER_TEST_H

#include <cmath>
#include <random>
#include <chrono>         // std::chrono::seconds

#include "graphics/ChartThread.h"
#include "graphics/PointChart.h"
#include "ml/DigitalFilter.h"
#include "ml/decisiontree/DecisionTree.h"
#include "ml/Entropy.h"
#include "utils/hash/StringHash.h"

#include "test.h"

# define MY_PI		3.14159265358979323846	/* pi */

namespace test {
	namespace ml_digital_filter {

		struct MyItem {
			int val1;
		};

		class MyMatcher: public ml::decisiontree::SplitMatcher<MyItem> {
			bool match(MyItem& itemA, MyItem& itemB) {
				return true;
			}
		};

		void digital_filter_test();

		void decision_tree_test();
	}
}

#endif
