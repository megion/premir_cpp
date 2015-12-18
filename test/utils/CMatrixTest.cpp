#include "CMatrixTest.h"

void test_set_value() {
    size_t rowSize = 4;
    size_t colSize = 3;
    utils::CMatrix<int> matrix(rowSize, colSize);

    for (size_t r = 0; r < rowSize; ++r) {
        int val = r * colSize;
        int values[] = {val, val + 1, val + 2};
        matrix.writeRow(r, values);
    }

    assert(matrix(2,2) == 8);
    assert(matrix(3,2) == 11);
}

void test_comparison() {
    utils::CMatrix<int> a(2, 2);
    utils::CMatrix<int> b(1, 2);

    assert(a != b);

    utils::CMatrix<int> c(2, 2);

    int row0[] = {1, 2};
    a.writeRow(0, row0);
    c.writeRow(0, row0);

    int row1[] = {5, 6};
    a.writeRow(1, row1);
    c.writeRow(1, row1);

    assert(a == c);

    c(1,1) = 8;
    assert(a != c);
}

void cMatrix_test() {
    suite("CMatrix");
    test(set_value);
    test(comparison);
}
