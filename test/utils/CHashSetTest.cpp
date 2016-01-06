#include "CHashSetTest.h"

namespace test {
    namespace utils_cHashSet {

        void test_hash_set() {


            ml::LevensteinDistance<int> ld;
            const wchar_t * str1 = L"моя руссская строка";
            const wchar_t * str2 = L"моя руrсская стро21";
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

            utils::CHashSet<char*, utils::hash::StringHash<char*>> s1;
            char ch1[] = "test";
            size_t h = s1.hash(ch1);
            std::cout << "hash: " << h << std::endl;

            utils::hash::FloatHash<double> hash2(1.2, 1.6);
            utils::CHashSet<double, utils::hash::FloatHash<double>> s2(hash2);
            size_t h2 = s2.hash(1.22222);
            std::cout << "hash2: " << h2 << std::endl;
            size_t h3 = s2.hash(1.233);
            std::cout << "hash2: " << h3 << std::endl;
        }

        void cHashSet_test() {
            suite("CHashSetTest");
            test(hash_set);
        }
    }
}



