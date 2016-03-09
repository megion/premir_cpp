#include "HashMapArrayTest.h"

namespace test {
    namespace utils_hashMapArray {

        void test_push_value() {
            utils::hash::CharHash shash;
            utils::HashMapArray<char, int> a(&shash, 6, 10);

            a.pushValue(0, 'K', 4);
            assert(a.getValue(0, 'K') != nullptr);
            assert((*a.getValue(0, 'K')) == 4);

            a.pushValue(35, 'A', 7);
            assert(a.getValue(35, 'K') == nullptr);
            assert((*a.getValue(35, 'A')) == 7);

            a.pushValue(0, 'K', 5);
            assert((*a.getValue(0, 'K')) == 5);

            a.pushValue(0, 'P', 55);
            assert((*a.getValue(0, 'K')) == 5);
            assert((*a.getValue(0, 'P')) == 55);
            assert(a.getValue(0, 'M') == nullptr);

            utils::hash::StringHash<char*> strHash;
            utils::HashMapArray<char*, int> b(&strHash, 6, 10);

            char mystr[] = "my string";
            b.pushValue(0, mystr, 4);
            assert(b.getValue(0, mystr) != nullptr);
            assert((*b.getValue(0, mystr)) == 4);

            b.pushValue(0, mystr, 42);
            assert((*b.getValue(0, mystr)) == 42);
        }

//        void test_init_in_constructor() {
//            int rNum = 160;
//            int cNum = 12;
//            utils::R3DMatrix<bool, bool, int> a(rNum, cNum);
//
//            int i = 0;
//            for (int r = 0; r < rNum; ++r) {
//                assert(a[r].cellSize == cNum);
//                for (int c = 0; c < cNum; ++c) {
//                    assert(a[r].cells[c].pointSize == 0);
//                    assert(a[r].cells[c].points == nullptr);
//
//                    int points[] = {1, 2};
//                    a.writePoints(r, c, points, 2);
//                    assert(a[r][c][1] == 2);
//                    ++i;
//                }
//            }
//
//            assert(i == rNum * cNum);
//        }
//
//        void test_write_to_end() {
//            utils::R3DMatrix<bool, bool, int> b(6, 0);
//            int points1[] = {1, 2};
//            b.writePoints(0, 4, points1, 2);
//            int points2[] = {11, 12};
//            b.writePoints(0, 4, points2, 2, true);
//            assert(b[0][4].pointSize == 4);
//            assert(b[0][4][0] == 1);
//            assert(b[0][4][3] == 12);
//
//            int points3[] = {33, 44, 66};
//            b.writePoints(10, 6, points3, 3, true);
//            assert(b[10][6].pointSize == 3);
//            b.writePoints(10, 6, points3, 3, true);
//            assert(b[10][6].pointSize == 6);
//            assert(b[10][6][0] == 33);
//            assert(b[10][6][5] == 66);
//
//        }

        void hashMapArray_test() {
            suite("HashMapArray");
            mytest(push_value);
//            mytest(comparison_with_error);
//            mytest(init_in_constructor);
//            mytest(write_to_end);
        }
    }
}



