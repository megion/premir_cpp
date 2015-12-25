#include "SMatrixTest.h"

namespace test {
    namespace smatrix {

        void test_push_row() {
            utils::SMatrix<int> a(2, 2);
            assert(a.getRowSize() == 2);
            assert(a.getColSize() == 2);

            int row0[] = {1, 2};
            a.writeRow(0, row0);
            int row1[] = {3, 4};
            a.writeRow(1, row1);
            assert(a.getRowSize() == 2);

            int row3[] = {5, 6};
            a.pushRow(row3);
            assert(a(2, 0) == 5);
            assert(a.getRowSize() == 3);
            a.pushRow(row3);
            assert(a(2, 1) == 6);
            assert(a(3, 0) == 5);

            a.writeRow(10, row3);
            assert(a.getRowSize() == 11);
            assert(a(10, 0) == 5);
            assert(a(3, 0) == 5);
        }

        void test_swap_rows() {
            utils::SMatrix<int> a(4, 5);

            int row0[] = {1, 2, 3, 4, 5};
            a.writeRow(0, row0);
            int row1[] = {6, 7, 8, 9, 10};
            a.writeRow(1, row1);
            int row2[] = {11, 12, 13, 14, 15};
            a.writeRow(2, row2);
            int row3[] = {16, 17, 18, 19, 20};
            a.writeRow(3, row3);

            a.swapRows(1, 2);

            assert(a(1,0) == 11);
            assert(a(1,4) == 15);
            assert(a(2,0) == 6);
            assert(a(2,4) == 10);
        }

        void sMatrix_test() {
            suite("SMatrix");
            test(push_row);
            test(swap_rows);
        }
    }
}



