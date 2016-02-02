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

        void test_heap_sort() {
            int arrA[] = {1, 2, 2, 8, 9, 3, 4, 5, 3, 3, 6, 7};
            utils::CArrayList<int> vectorA;
            vectorA.push(arrA, 12);

            sort::HeapSort<int> hsort;
            hsort.sort(vectorA.getArray(), vectorA.size());
            vectorA.print();


            int arrB[] = {1, 2, 2, 3, 3, 3, 4, 5, 6, 7, 8, 9};
            utils::CArrayList<int> vectorB;
            vectorB.push(arrB, 12);

            assert(vectorA == vectorB);
        }

        void test_median() {
            // четный случай
            int arrA[] = {1, 2, 2, 8, 9, 3, 4, 5, 3, 3, 6, 7};
            double med = utils::ArrayUtils<int, int, double>::median(arrA, 12);
            assert(med == 3.5);

            // нечетный случай
            int arrB[] = {12, 1, 2, 2, 8, 9, 3, 4, 5, 3, 3, 6, 7};
            double med2 = utils::ArrayUtils<int, int, double>::median(arrB, 13);
            assert(med2 == 4);
        }

        void test_average() {
            // четный случай
            int arrA[] = {1, 2, 2, 8, 9, 3, 4, 5, 3, 3, 6, 7};
            double aver = utils::ArrayUtils<int, int, double>::average(arrA, 12);
            assert_range(aver, 4.41667, 0.1);
        }

        void arrayutils_test() {
            suite("ArrayUtils");
            mytest(norm_euclidean_vector);
            mytest(scalar_multiplication_arrays);
            mytest(heap_sort);
            mytest(median);
            mytest(average);
        }
    }
}



