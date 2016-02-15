#include "RDMatrixTest.h"

namespace test {
    namespace utils_rdmatrix {

        void test_push_row() {
            utils::RDMatrix<double, int> a;

            int points0[] = {1, 2};
            utils::RDMatrix<double, int>::Row row0;
            row0.points = points0;
            row0.colSize = 2;
            a.writeRow(0, row0);

            int points1[] = {3, 4};
            a.writeRow(1, points1, 2);

            // число строк = 2
            assert(a.getRowSize() == 2);

            int points3[] = {5, 6, 7};
            utils::RDMatrix<double, int>::Row row3;
            row3.points = points3;
            row3.colSize = 3;
            a.pushRow(row3);

            assert(a(2, 0) == 5);
            assert(a(2, 2) == 7);
            assert(a.getRowSize() == 3);

            a.pushRow(points3, 3);
            assert(a(2, 1) == 6);
            assert(a(3, 0) == 5);
            assert(a(3, 2) == 7);

            a.writeRow(10, row3);
            assert(a.getRowSize() == 11);
            assert(a(10, 0) == 5);
            assert(a(3, 0) == 5);
            assert(a(3, 2) == 7);

            a.writeRow(16, points3, 3);
            assert(a.getRowSize() == 17);
            assert(a(16, 0) == 5);
            assert(a(3, 2) == 7);

            int points4[] = {5, 6, 7, 8};
            // replace exist
            a.writeRow(16, points4, 4);
            assert(a.getRowSize() == 17);
            assert(a(16, 0) == 5);
            assert(a(16, 3) == 8);
        }

        void test_comparison() {
            utils::RDMatrix<double, int> a;
            utils::RDMatrix<double, int> b;

            assert(a == b);

            utils::RDMatrix<double, int> c;

            int points0[] = {1, 2};
            utils::RDMatrix<double, int>::Row row0;
            row0.points = points0;
            row0.data = 2;
            row0.colSize = 2;
            a.writeRow(0, row0);
            c.writeRow(0, row0);

            int points1[] = {5, 6};
            utils::RDMatrix<double, int>::Row row1;
            row1.points = points1;
            row1.data = 4;
            row1.colSize = 2;
            a.writeRow(1, row1);
            c.writeRow(1, row1);

            assert(a == c);

            c(1,1) = 8;
            assert(a != c);
        }

        void test_comparison_with_error() {
            utils::RDMatrix<double, int> a;
            utils::RDMatrix<double, int> c;

            int points0[] = {1, 2};
            utils::RDMatrix<double, int>::Row row0;
            row0.points = points0;
            row0.data = 2;
            row0.colSize = 2;
            a.writeRow(0, row0);
            c.writeRow(0, row0);

            int points1[] = {5, 6};
            utils::RDMatrix<double, int>::Row row1;
            row1.points = points1;
            row1.data = 4;
            row1.colSize = 2;
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
            utils::RDMatrix<double, int> a;

            int row0[] = {1, 1, 1, 1};
            a.writeRow(0, row0, 4);
            a.writeRow(1, row0, 4);

            for (size_t r=0; r<a.getRowSize(); ++r) {
                utils::RDMatrix<double, int>::Row& row = a.getRow(r);
                for (size_t i=0; i<row.colSize; ++i) {
                    assert(row.points[i]==1);
                }
            }
        }

        void rDMatrix_test() {
            suite("RDMatrix");
            mytest(push_row);
            mytest(comparison);
            mytest(comparison_with_error);
            mytest(get_row);
        }
    }
}



