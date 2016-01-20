#include "ArrayUtilsTest.h"

namespace test {
    namespace utils_arrayutils {

        void test_norm_euclidean_vector() {
            double arrA[] = {1, 2, 2};
            utils::CArrayList<double> vectorA;
            vectorA.push(arrA, 3);
            double res = utils::ArrayUtils<double, double, double>::
            euclideanNorm(vectorA.getArray(), vectorA.size());
            assert(res == 3);
        }

        void test_scalar_multiplication_arrays() {
            double arrA[] = {1, 2, 3};
            utils::CArrayList<double> vectorA;
            vectorA.push(arrA, 3);
            double arrB[] = {3, 2, 4};
            utils::CArrayList<double> vectorB;
            vectorB.push(arrB, 3);
            double res = utils::ArrayUtils<double, double, double>::
            scalarMultiplication(vectorA.getArray(), vectorB.getArray(),
                                 vectorA.size());
            assert(res == 19);
        }

        void arrayutils_test() {
            suite("ArrayUtils");
            test(norm_euclidean_vector);
            test(scalar_multiplication_arrays);
        }
    }
}



