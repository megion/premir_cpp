#include <HashEngineTest.h>

namespace test {
    namespace utils_hash_engine {

        void test_hash_engine() {

            utils::hash::StringHash<char *> shash;
            char ch1[] = "test";
            size_t h = shash.hashCode(ch1);
//            std::cout << "hash: " << h << std::endl;

            utils::hash::FloatHash<double> hash2(1.2, 1.6);
            size_t h2 = hash2.hashCode(1.22222);
//            std::cout << "hash2: " << h2 << std::endl;
            size_t h3 = hash2.hashCode(1.233);
//            std::cout << "hash2: " << h3 << std::endl;
        }

//        void test_number_hash_charts() {
//            graphics::PointChart numberHashChart(false, 710, 460);
//            numberHashChart.setWindowTitle("Number hash");
//            graphics::ChartThread<bool> numberHashChartThread(&numberHashChart);
//
//            utils::hash::NumberHash<double> nflash;
////            utils::CHashSet<double> numSet(&nflash);
//            for (double i = -100; i < 100; i = i + 0.1) {
//                size_t h = nflash.hashCode(i);
//                numberHashChart.redrawNewPoint(0, i, h);
//                std::this_thread::sleep_for(std::chrono::milliseconds(2));
//            }
//        }
//
//        void test_float_hash_charts() {
//            graphics::PointChart floatHashChart(false, 710, 460);
//            floatHashChart.setWindowTitle("Float hash");
//            graphics::ChartThread<bool> floatHashChartThread(&floatHashChart);
//
//            utils::hash::FloatHash<double> fhash(1.2, 1.6);
//            utils::CHashSet<double> floatSet(&fhash);
//            for (double i = 1.2; i < 1.6; i = i + 0.0001) {
//                size_t h = floatSet.hash(i);
//                floatHashChart.redrawNewPoint(0, i, h);
//                std::this_thread::sleep_for(std::chrono::milliseconds(2));
//            }
//        }

        void hashEngine_test() {
            suite("HashEngine");
            mytest(hash_engine);
//            mytest(number_hash_charts);
//            mytest(float_hash_charts);
        }
    }
}



