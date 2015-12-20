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

void test_row_iterator() {
    utils::CMatrix<int> matrix(2, 2);

    int row0[] = {1, 2};
    matrix.writeRow(0, row0);
    int row1[] = {3, 4};
    matrix.writeRow(1, row1);

    assert(matrix(1,1) == 4);

    int i = 0;
    for (int& rv : matrix.row(1)) {
        if (i==0) {
            assert(rv == 3);
        } else if (i==1) {
            assert(rv == 4);
        }
        ++i;
    }
    assert(i == 2);

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

void test_push_row() {
    utils::CMatrix<int> a(2, 2);
    assert(a.getRowSize() == 2);
    assert(a.getColSize() == 2);

    int row3[] = {5, 6};
    a.pushRow(row3);
    assert(a.getRowSize() == 3);

    // test last pushed row
    int i = 0;
    for (int& rv : a.row(2)) {
        if (i==0) {
            assert(rv == 5);
        } else if (i==1) {
            assert(rv == 6);
        }
        ++i;
    }
    assert(i == 2);
}

void cMatrix_test() {
    suite("CMatrix");
    test(set_value);
    test(comparison);
    test(push_row);
    test(row_iterator);

}
