#include "HashEngineTest.h"

namespace test {
    namespace utils_hash_engine {

        void test_simple_array_list_hash() {

			utils::CArrayList<long> arr1;
			utils::CArrayList<long> arr2;

			utils::hash::ArrayListSimpleTypeHash<long> hashEngine1;
			utils::hash::ArrayListSimpleTypeHash<long> hashEngine2;
			//hashEngine.hashIndex(arr1);

			//graphics::PointChart numberHashChart(false, 710, 460);
			//numberHashChart.setWindowTitle("Number hash");
			//graphics::ChartThread<bool> numberHashChartThread(&numberHashChart);

			for (size_t i = 0; i < 1000; ++i) {
				arr1.push(i);
				arr2.push(i);
				size_t h1 = hashEngine1.hashIndex(arr1);
				size_t h2 = hashEngine2.hashIndex(arr2);

				assert(h1 == h2);
				//numberHashChart.redrawNewPoint(0, i, h);
				//std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}

        }

        void hashEngine_test() {
            suite("HashEngine");
			mytest(simple_array_list_hash);
        }
    }
}



