#include "matrix_utils_test.h"

void test_multiply_matrix() {

//    size_t nSize = 3;
    utils::CMatrix<double> matrixA(4, 3);

    // set values to A
    double row0[] = {0, 1, 2};
    matrixA.writeRow(0, row0);
    double row1[] = {3, 4, 5};
    matrixA.writeRow(1, row1);
    double row2[] = {6, 7, 8};
    matrixA.writeRow(2, row2);
    double row3[] = {9, 10, 11};
    matrixA.writeRow(3, row3);

    utils::CMatrix<double> matrixB(3, 4);
    double rowB0[] = {0, 1, 2, 3};
    matrixB.writeRow(0, rowB0);
    double rowB1[] = {4, 5, 6, 7};
    matrixB.writeRow(1, rowB1);
    double rowB2[] = {8, 9, 10, 11};
    matrixB.writeRow(2, rowB2);

    utils::CMatrix<double> *matrixC = utils::multiplyMatrix(matrixA, matrixB, 1,
                                                            1,
                                                            1, 1, 2, 2, 2, 2);

    utils::CMatrix<double> matrixCheck(2, 2);
    double rowCheck0[] = {65, 74};
    matrixCheck.writeRow(0, rowCheck0);
    double rowCheck1[] = {107, 122};
    matrixCheck.writeRow(1, rowCheck1);

    assert(matrixCheck == (*matrixC));


    delete matrixC; // do not remember delete resource
}

void test_distance_vectors() {
    double arrA[] = {1, 2, 1};
    utils::CArrayList<double> vectorA;
    vectorA.push(arrA, 3);
    double arrB[] = {3, 4, 0};
    utils::CArrayList<double> vectorB;
    vectorB.push(arrB, 3);
    double res = utils::distanceArrays(vectorA.getArray(), vectorB.getArray(),
                                       vectorA.size());

    assert(res == 3.0);
}




//void test_cos_angel_vectors() {
//    double arrA[] = {1, 2, 1};
//    utils::CArrayList<double> vectorA;
//    vectorA.push(arrA, 3);
//    double arrB[] = {3, 4, 0};
//    utils::CArrayList<double> vectorB;
//    vectorB.push(arrB, 3);
//    double res = utils::cosAngelVectors(vectorA, vectorB);
//    assert(res == 3);
//}

void matrix_utils_test() {
    suite("matrix_utils");
    test(multiply_matrix);
    test(distance_vectors);
}
