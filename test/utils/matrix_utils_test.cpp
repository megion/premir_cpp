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

    utils::CMatrix<double> *matrixC = utils::multiplyMatrix(matrixA, matrixB, 1, 1,
                                                         1, 1, 2, 2, 2, 2);

    utils::CMatrix<double> matrixCheck(2, 2);
    double rowCheck0[] = {65, 74};
    matrixCheck.writeRow(0, rowCheck0);
    double rowCheck1[] = {107, 122};
    matrixCheck.writeRow(1, rowCheck1);

    assert(matrixCheck == (*matrixC));


    delete matrixC; // do not remember delete resource
}

void test_scalar_multiply_vectors() {
    double arrA[] = {1, 2, 3};
    utils::CArrayList<double> vectorA;
    vectorA.push(arrA, 3);
    double arrB[] = {3, 2, 4};
    utils::CArrayList<double> vectorB;
    vectorB.push(arrB, 3);
    double res = utils::multiplyArrays(vectorA.getArray(), vectorB.getArray(), vectorA.size());

    assert(res == 19);
}

void test_distance_vectors() {
    double arrA[] = {1, 2, 1};
    utils::CArrayList<double> vectorA;
    vectorA.push(arrA, 3);
    double arrB[] = {3, 4, 0};
    utils::CArrayList<double> vectorB;
    vectorB.push(arrB, 3);
    double res = utils::distanceArrays(vectorA.getArray(), vectorB.getArray(), vectorA.size());

    assert(res == 3.0);
}

void test_norm_euclidean_vector() {
    double arrA[] = {1, 2, 2};
    utils::CArrayList<double> vectorA;
    vectorA.push(arrA, 3);
    double res = utils::euclideanNorm(vectorA.getArray(), vectorA.size());
    assert(res == 3);
}


void test_gram_schmidt_vector_basis() {
//    utils::CMatrix<double> matrix(0, 3);
    ml::GramSchmidtBasis basis(3);
//    for (size_t r = 0; r < 10; ++r) {
//        for (size_t c = 0; c < 20; ++c) {
//            matrix(r, c) = r * 20 + c;
//        }
//    }
//    (1−11)(101)(112)
    // [-2,1,0],[-2,0,1],[-0.5,-1,1]
    double d[] = {1,-1,1};
//    matrix.pushRow(d);
    basis.pushInVector(d);
    double d1[] = {1,0,1};
//    matrix.pushRow(d1);
    basis.pushInVector(d1);
    double d2[] = {1,1,2};
    basis.pushInVector(d2);
//    matrix.pushRow(d2);
    basis.getOutVectors()->printMatrix();
//    utils::CMatrix<double> *res = utils::gramSchmidtVectorBasis(matrix);
//    res->printMatrix();
//    assert(res->getRowSize() == 3);
//    assert(res->getColSize() == 3);
//    delete res;
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
    test(scalar_multiply_vectors);
    test(distance_vectors);
//    test(cos_angel_vectors);
    test(norm_euclidean_vector);
    test(gram_schmidt_vector_basis);
}
