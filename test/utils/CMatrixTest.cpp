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

void test_comparison_with_error() {
    utils::CMatrix<int> a(2, 2);
    utils::CMatrix<int> c(2, 2);

    int row0[] = {1, 2};
    a.writeRow(0, row0);
    c.writeRow(0, row0);

    int row1[] = {5, 6};
    a.writeRow(1, row1);
    c.writeRow(1, row1);

    assert(a.equalsWithError(c, 1));
    assert(a.equalsWithError(c, 0));
    assert(!a.equalsWithError(c, -1));

    c(1,1) = 8;
    assert(!a.equalsWithError(c, 1));
    assert(a.equalsWithError(c, 2));
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

void test_create_clone() {
    utils::CMatrix<int> a(4, 5);

    int row0[] = {1, 2, 3, 4, 5};
    a.writeRow(0, row0);
    int row1[] = {6, 7, 8, 9, 10};
    a.writeRow(1, row1);
    int row2[] = {11, 12, 13, 14, 15};
    a.writeRow(2, row2);
    int row3[] = {16, 17, 18, 19, 20};
    a.writeRow(3, row3);

    utils::CMatrix<int>* cl1 = a.createClone(1, 1, 2, 3);
    assert(cl1->getRowSize() == 2);
    assert(cl1->getColSize() == 3);
    assert((*cl1)(0,0) == 7);
    assert((*cl1)(0,2) == 9);
    assert((*cl1)(1,0) == 12);
    assert((*cl1)(1,2) == 14);

    utils::CMatrix<int>* cl2 = a.createClone(1, 3);
    assert(cl2->getRowSize() == 3);
    assert(cl2->getColSize() == 2);
    assert((*cl2)(0,0) == 9);

    utils::CMatrix<int>* cl3 = a.createClone(); // full copy
    assert(cl3->getRowSize() == 4);
    assert(cl3->getColSize() == 5);
    assert((*cl3)(0,0) == 1);

    delete cl1;
    delete cl2;
    delete cl3;
}

void test_swap_rows() {
    utils::CMatrix<int> a(4, 5);

    int row0[] = {1, 2, 3, 4, 5};
    a.writeRow(0, row0);
    int row1[] = {6, 7, 8, 9, 10};
    a.writeRow(1, row1);
    int row2[] = {11, 12, 13, 14, 15};
    a.writeRow(2, row2);
    int row3[] = {16, 17, 18, 19, 20};
    a.writeRow(3, row3);

    a.swapRows(1, 2);

    assert(a(1,0) == 11);
    assert(a(1,4) == 15);
    assert(a(2,0) == 6);
    assert(a(2,4) == 10);
}

void test_make_identity() {
    utils::CMatrix<int> a(4, 4);

    a.makeIdentity();

    utils::CMatrix<int> b(4, 4);
    int row0[] = {1, 0, 0, 0};
    b.writeRow(0, row0);
    int row1[] = {0, 1, 0, 0};
    b.writeRow(1, row1);
    int row2[] = {0, 0, 1, 0};
    b.writeRow(2, row2);
    int row3[] = {0, 0, 0, 1};
    b.writeRow(3, row3);

    assert(a == b);
}

void cMatrix_test() {
    suite("CMatrix");
    mytest(set_value);
    mytest(comparison);
    mytest(comparison_with_error);
    mytest(push_row);
    mytest(row_iterator);
    mytest(create_clone);
    mytest(swap_rows);
    mytest(make_identity);
}
