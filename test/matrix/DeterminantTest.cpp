#include "DeterminantTest.h"

namespace test {
    namespace matrix_determinant {
        void test_gauss_determinant() {
            utils::SMatrix<double> a(4, 4);
            double row0[] = {3, 2, 3, 4};
            a.writeRow(0, row0);
            double row1[] = {4, 4, 3, 2};
            a.writeRow(1, row1);
            double row2[] = {1, 4, 4, 3};
            a.writeRow(2, row2);
            double row3[] = {2, 3, 1, 1};
            a.writeRow(3, row3);

            matrix::Determinant<double, double> d;
            double v = d.gaussDeterminant(a);

            assert( (45-0.1 < v) && (v < 45+0.1));
        }

        void test_bareiss_determinant() {
            utils::SMatrix<double> a(4, 4);
            double row0[] = {3, 2, 3, 4};
            a.writeRow(0, row0);
            double row1[] = {4, 4, 3, 2};
            a.writeRow(1, row1);
            double row2[] = {1, 4, 4, 3};
            a.writeRow(2, row2);
            double row3[] = {2, 3, 1, 1};
            a.writeRow(3, row3);

            matrix::Determinant<double, int> d;
            int v = d.bareissDeterminant(a);
            assert(v==45);
        }

        void determinant_test() {
            suite("determinant_test");
            test(gauss_determinant);
            test(bareiss_determinant);
        }
    }
}



