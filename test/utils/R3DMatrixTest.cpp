#include "R3DMatrixTest.h"

namespace test {
    namespace utils_r3dmatrix {

        void test_write_row() {
            utils::R3DMatrix<bool, bool, int> a;

            int points0[] = {1, 2};
            a.writePoints(0, 0, points0, 2);

            int points1[] = {3, 4};
            a.writePoints(1, 2, points1, 2);

            // число строк = 2
            assert(a.getRowSize() == 2);
            assert(a.getRow(1).cellSize == 3);
            assert(a.getRow(1).cells[2].pointSize == 2);
            assert(a.getRow(1).cells[2].points[1] == 4);


            int points3[] = {5, 6, 7};
            a.writePoints(5, 6, points3, 3);
            assert(a.getRowSize() == 6);
            assert(a.getRow(1).cellSize == 3);
            assert(a.getRow(5).cells[6].pointSize == 3);
            assert(a(5, 6, 2) == 7);

            int points4[] = {8, 9, 10};
            a.writePoints(5, 3, points4, 3);
            assert(a.getRowSize() == 6);
            assert(a.getRow(1).cellSize == 3);
            assert(a.getRow(5).cells[6].pointSize == 3);
            assert(a(5, 6, 2) == 7);
            assert(a(5, 3, 2) == 10);

            int points5[] = {11, 12, 13};
            a.writePoints(5, 7, points5, 3);
            assert(a.getRowSize() == 6);
            assert(a(5, 6, 2) == 7);
            assert(a(5, 3, 2) == 10);
            assert(a(5, 7, 1) == 12);

            int points6[] = {5, 6, 7, 8};
            // replace exist
            a.writePoints(5, 7, points6, 4);
            assert(a(5, 6, 2) == 7);
            assert(a(5, 3, 2) == 10);
            assert(a(5, 7, 1) == 6);
            assert(a(5, 7, 3) == 8);
        }

        void test_comparison_with_error() {
            utils::R3DMatrix<bool, bool, int> a;
            utils::R3DMatrix<bool, bool, int> c;

            assert(a.equalsWithError(c, 0, true));

            int points0[] = {1, 2};
            a.writePoints(0, 0, points0, 2);
            c.writePoints(0, 0, points0, 2);

            assert(a.equalsWithError(c, 0, true));

            a.writePoints(4, 5, points0, 2);
            c.writePoints(4, 5, points0, 2);

            assert(a.equalsWithError(c, 0, true));

            c.writePoints(3, 3, points0, 2);

            assert(!a.equalsWithError(c, 0, true));
        }

        void test_init_in_constructor() {
            int rNum = 160;
            int cNum = 12;
            utils::R3DMatrix<bool, bool, int> a(rNum, cNum);

            int i = 0;
            for (int r = 0; r < rNum; ++r) {
                assert(a[r].cellSize == cNum);
                for (int c = 0; c < cNum; ++c) {
                    assert(a[r].cells[c].pointSize == 0);
                    assert(a[r].cells[c].points == nullptr);

                    int points[] = {1, 2};
                    a.writePoints(r, c, points, 2);
                    assert(a[r][c][1] == 2);
                    ++i;
                }
            }

            assert(i == rNum * cNum);
        }

        void r3DMatrix_test() {
            suite("R3DMatrix");
            mytest(write_row);
            mytest(comparison_with_error);
            mytest(init_in_constructor);
        }
    }
}



