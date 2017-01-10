#include <HashEngineTest.h>

namespace test {
    namespace utils_hash_engine {

        void test_hash_engine() {

            utils::hash::StringHash<char *> shash;
            char ch1[] = "test";
            size_t h = shash.hashCode(ch1);
//            std::cout << "hash: " << h << std::endl;

        }

		void test_number_hash_charts() {
			graphics::PointChart numberHashChart(false, 710, 460);
			numberHashChart.setWindowTitle("Number hash");
			graphics::ChartThread<bool> numberHashChartThread(&numberHashChart);

			utils::hash::NumberHash<double> nflash;
//            utils::CHashSet<double> numSet(&nflash);
			for (double i = -100; i < 100; i = i + 0.1) {
				size_t h = nflash.hashCode(i);
				numberHashChart.redrawNewPoint(0, i, h);
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}
		}

        void hashEngine_test() {
            suite("HashEngine");
            mytest(hash_engine);
			//mytest(number_hash_charts);
        }
    }
}



