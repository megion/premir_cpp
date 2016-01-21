#include "GramSchmidtTest.h"

namespace test {
    namespace matrix_gramSchmidt {

        void test_gram_schmidt_strteam() {

            // [1,-1,1],[1,0,1],[1,1,2]
            utils::SMatrix<double> dataMatrix(0, 3);
            double d[] = {1, -1, 1};
            dataMatrix.pushRow(d);
            double d1[] = {1, 0, 1};
            dataMatrix.pushRow(d1);
            double d2[] = {1, 1, 2};
            dataMatrix.pushRow(d2);

            matrix::GramSchmidtStream<double, double> basis(3);
            for (size_t r = 0; r < dataMatrix.getRowSize(); ++r) {
                double *row = dataMatrix.getRow(r);
                basis.nextVector(row);
            }

            // матрица проверки
            utils::SMatrix<double> matrixCheck(3, 3);
            double rowCheck0[] = {1, -1, 1};
            matrixCheck.writeRow(0, rowCheck0);
            double rowCheck1[] = {1.0 / 3.0, 2.0 / 3.0, 1.0 / 3.0};
            matrixCheck.writeRow(1, rowCheck1);
            double rowCheck2[] = {-0.5, 0, 0.5};
            matrixCheck.writeRow(2, rowCheck2);

            basis.getOutVectors()->print();

            // test with error
            assert(basis.getOutVectors()->equalsWithError(matrixCheck,
                                                          0.00001));
        }

        void test_gram_schmidt_normalized() {
            // [1,-1,1],[1,0,1],[1,1,2]
            utils::SMatrix<double> dataMatrix(3, 3);
            double d[] = {1, -1, 1};
            dataMatrix.writeRow(0, d);
            double d1[] = {1, 0, 1};
            dataMatrix.writeRow(1, d1);
            double d2[] = {1, 1, 2};
            dataMatrix.writeRow(2, d2);

            matrix::GramSchmidtNormalized<double, double> basis;
            basis.gramSchmidt(dataMatrix);

//            0.577350, -0.577350, 0.577350,
//                    0.408248, 0.816497, 0.408248,
//                    -0.707107, -0.000000, 0.707106

            dataMatrix.print();
        }

        void gramSchmidt_test() {
            suite("GramSchmidt");
            test(gram_schmidt_strteam);
            test(gram_schmidt_normalized);
        }
    }
}



