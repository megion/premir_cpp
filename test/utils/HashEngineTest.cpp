#include "HashEngineTest.h"

namespace test {
    namespace utils_hash_engine {

        void test_simple_array_list_hash() {

			utils::CArrayList<long> arr1;
			//arr1.push(1);
			//arr1.push(2);
			//arr1.push(123456);
			//arr1.push(34535667);
			//arr1.push(23535345354561);

			utils::hash::ArrayListSimpleTypeHash<long> hashEngine;
			//hashEngine.hashIndex(arr1);

			graphics::PointChart numberHashChart(false, 710, 460);
			numberHashChart.setWindowTitle("Number hash");
			graphics::ChartThread<bool> numberHashChartThread(&numberHashChart);

			for (size_t i = 0; i < 1000; ++i) {
				arr1.push(i);
				size_t h = hashEngine.hashIndex(arr1);

				numberHashChart.redrawNewPoint(0, i, h);
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}

        }


        void test_hash_engine() {

            //utils::hash::HashEngine<char*> shash;
            //char ch1[] = "test";
            //size_t h = shash.hashCode(ch1);
//            std::cout << "hash: " << h << std::endl;

        }

		//void test_number_hash_charts() {
			
		//}

        void hashEngine_test() {
            suite("HashEngine");
			mytest(simple_array_list_hash);
            //mytest(hash_engine);
			//mytest(number_hash_charts);
        }
    }
}



