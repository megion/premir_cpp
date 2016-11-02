#include "LevensteinDistanceTest.h"

namespace test {
    namespace ml_levenstein_distance {

        void test_compare_distance() {

            ml::LevensteinDistance<int> ld;
            const wchar_t *str1 = L"моя руссская строка";
            const wchar_t *str2 = L"моя руrсская стро21";
            int d1 = ld.wld(str1, str2, wcslen(str1), wcslen(str2));
            int d2 = ld.wldOptimazed(str1, str2, wcslen(str1), wcslen(str2));
//            std::cout << "d1: " << d1 << std::endl;
//            std::cout << "d2: " << d2 << std::endl;
            assert(d1 == d2);
        }

        void livenstein_distance_test() {
            suite("LevensteinDistance");
            mytest(compare_distance);
        }
    }
}



