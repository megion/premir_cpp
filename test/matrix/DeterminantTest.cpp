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

//            a.print();

            assert_range(v, 45, 0.1);

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

        void test_by_hilbert_matrix() {
            utils::SMatrix<double> a(5, 5);
            for (size_t i = 0; i<5; ++i) {
                for (size_t j = 0; j<5; ++j) {
                    a(i,j) = 1.0/ (double)(i + j + 1.0);
                }
            }

            utils::SMatrix<double>* b = a.createClone();

            matrix::Determinant<double, double> d;
            double v1 = d.bareissDeterminant(a);
            assert_range(1.0/v1, 266716800000, 1);

            double v2 = d.gaussDeterminant(*b);
            assert_range(1.0/v2, 266716800000, 1);
        }

        void determinant_test() {
            suite("determinant_test");
            test(gauss_determinant);
            test(bareiss_determinant);
            test(by_hilbert_matrix);
        }
    }
}



