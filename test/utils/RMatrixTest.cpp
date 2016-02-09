#include "RMatrixTest.h"

namespace test {
    namespace utils_rmatrix {

        void test_push_row() {
            utils::RMatrix<double, int> a(2, 2);
            assert(a.getRowSize() == 2);
            assert(a.getColSize() == 2);

            int points0[] = {1, 2};
            utils::RMatrix<double, int>::Row row0;
            row0.points = points0;
            a.writeRow(0, row0);

            int points1[] = {3, 4};
            utils::RMatrix<double, int>::Row row1;
            row1.points = points1;
            a.writeRow(1, row1);

            // число строк не должно измениться
            assert(a.getRowSize() == 2);

            int points3[] = {5, 6};
            utils::RMatrix<double, int>::Row row3;
            row3.points = points3;
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
            utils::RMatrix<double, int> a(4, 5);

            int points0[] = {1, 2, 3, 4, 5};
            utils::RMatrix<double, int>::Row row0;
            row0.points = points0;
            a.writeRow(0, row0);

            int points1[] = {6, 7, 8, 9, 10};
            utils::RMatrix<double, int>::Row row1;
            row1.points = points1;
            a.writeRow(1, row1);

            int points2[] = {11, 12, 13, 14, 15};
            utils::RMatrix<double, int>::Row row2;
            row2.points = points2;
            a.writeRow(2, row2);

            int points3[] = {16, 17, 18, 19, 20};
            utils::RMatrix<double, int>::Row row3;
            row3.points = points3;
            a.writeRow(3, row3);

            a.swapRows(1, 2);

            assert(a(1,0) == 11);
            assert(a(1,4) == 15);
            assert(a(2,0) == 6);
            assert(a(2,4) == 10);
        }

        void test_comparison() {
            utils::RMatrix<double, int> a(2, 2);
            utils::RMatrix<double, int> b(1, 2);

            assert(a != b);

            utils::RMatrix<double, int> c(2, 2);

            int points0[] = {1, 2};
            utils::RMatrix<double, int>::Row row0;
            row0.points = points0;
            row0.data = 2;
            a.writeRow(0, row0);
            c.writeRow(0, row0);

            int points1[] = {5, 6};
            utils::RMatrix<double, int>::Row row1;
            row1.points = points1;
            row1.data = 4;
            a.writeRow(1, row1);
            c.writeRow(1, row1);

            assert(a == c);

            c(1,1) = 8;
            assert(a != c);
        }

        void test_comparison_with_error() {
            utils::RMatrix<double, int> a(2, 2);
            utils::RMatrix<double, int> c(2, 2);

            int points0[] = {1, 2};
            utils::RMatrix<double, int>::Row row0;
            row0.points = points0;
            row0.data = 2;
            a.writeRow(0, row0);
            c.writeRow(0, row0);

            int points1[] = {5, 6};
            utils::RMatrix<double, int>::Row row1;
            row1.points = points1;
            row1.data = 4;
            a.writeRow(1, row1);
            c.writeRow(1, row1);

            assert(a.equalsWithError(c, 1));
            assert(a.equalsWithError(c, 0));
            assert(!a.equalsWithError(c, -1));

            c(1,1) = 8;
            assert(!a.equalsWithError(c, 1));
            assert(a.equalsWithError(c, 2));
        }

        void test_get_row() {
            utils::RMatrix<double, int> a(2, 4);

            int row0[] = {1, 1, 1, 1};
            a.writeRow(0, row0);
            a.writeRow(1, row0);

            int* row = a.getRow(0).points;
            for (size_t i=0; i<a.getColSize(); ++i) {
                assert(row[i]==1);
            }
        }

        void test_copy_to() {
            utils::RMatrix<double, int> a(2, 4);
            int points0[] = {1, 1, 1, 1};
            utils::RMatrix<double, int>::Row row0;
            row0.points = points0;
            row0.data = 2;
            a.writeRow(0, row0);
            a.writeRow(0, row0);

            utils::RMatrix<double, int> b(2, 4);
            int rowB0[] = {2, 2, 2, 2};
            b.writeRow(0, rowB0);

            assert(a!=b);

            a.copyTo(b);

            assert(a==b);
        }

        void test_create_clone() {
            utils::RMatrix<double, int> a(4, 5);

            int row0[] = {1, 2, 3, 4, 5};
            a.writeRow(0, row0);
            int row1[] = {6, 7, 8, 9, 10};
            a.writeRow(1, row1);
            int row2[] = {11, 12, 13, 14, 15};
            a.writeRow(2, row2);
            int row3[] = {16, 17, 18, 19, 20};
            a.writeRow(3, row3);

            utils::RMatrix<double, int>* cl3 = a.createClone(); // full copy
            assert((*cl3)(0,0) == 1);
            assert(a == (*cl3));

            delete cl3;
        }

        void rMatrix_test() {
            suite("RMatrix");
            mytest(push_row);
            mytest(swap_rows);
            mytest(comparison);
            mytest(comparison_with_error);
            mytest(create_clone);
            mytest(copy_to);
            mytest(get_row);
        }
    }
}



