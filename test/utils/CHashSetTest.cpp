#include "CHashSetTest.h"

namespace test {
    namespace utils_cHashSet {

        void test_hash_set() {


            ml::LevensteinDistance<int> ld;
            const wchar_t *str1 = L"моя руссская строка";
            const wchar_t *str2 = L"моя руrсская стро21";
            int d1 = ld.wld(str1, str2, wcslen(str1), wcslen(str2));
            int d2 = ld.wldOptimazed(str1, str2, wcslen(str1), wcslen(str2));
//            std::cout << "d1: " << d1 << std::endl;
//            std::cout << "d2: " << d2 << std::endl;


//            const wchar_t * str = L"моя руссская строка";
//            wprintf(L"%ls\n", str);
//            wprintf(L"Первая буква Большая!\n");
//            for (size_t i = 0; i<1000; ++i) {
////                wprintf(&str[i]);
////                wprintf(L"Привет\0\0");
////                wprintf (L"str[%d]: %lc\n", i, str[i]);
////                std::wcout << str[i] << std::endl;// <<std::wstring(L"Привет");
//                std::cout << "[" << i << "]: " << i % 64 << std::endl;
//            }

            utils::CHashSet<char *, utils::hash::StringHash<char *>> s1;
            char ch1[] = "test";
            size_t h = s1.hash(ch1);
//            std::cout << "hash: " << h << std::endl;

            utils::hash::FloatHash<double> hash2(1.2, 1.6);
            utils::CHashSet<double, utils::hash::FloatHash<double>> s2(hash2);
            size_t h2 = s2.hash(1.22222);
//            std::cout << "hash2: " << h2 << std::endl;
            size_t h3 = s2.hash(1.233);
//            std::cout << "hash2: " << h3 << std::endl;
        }

        void test_number_hash_charts() {
            graphics::ChartThread numberHashChart(710, 460);
            numberHashChart.getChart().setWindowTitle("Number hash");
            graphics::ChartThread floatHashChart(710, 460);
            floatHashChart.getChart().setWindowTitle("Float hash");

            utils::CHashSet<double, utils::hash::NumberHash<double>> numSet;
            for (double i = -1000; i < 1000; i = i + 0.1) {
                size_t h = numSet.hash(i);
                numberHashChart.getChart().redrawNewPoints(i, h);
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }

        void test_float_hash_charts() {
            graphics::ChartThread floatHashChart(710, 460);
            floatHashChart.getChart().setWindowTitle("Float hash");

            utils::hash::FloatHash<double> fhash(1.2, 1.6);
            utils::CHashSet<double, utils::hash::FloatHash<double>> floatSet(
                    fhash);
            for (double i = 1.2; i < 1.6; i = i + 0.0001) {
                size_t h = floatSet.hash(i);
                floatHashChart.getChart().redrawNewPoints(i, h);
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }

        void cHashSet_test() {
            suite("CHashSetTest");
            mytest(hash_set);
//            test(number_hash_charts);
//            test(float_hash_charts);
        }
    }
}



