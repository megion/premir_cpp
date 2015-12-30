#include "SMatrixTest.h"

namespace test {
    namespace utils_smatrix {

        void test_push_row() {
            utils::SMatrix<int> a(2, 2);
            assert(a.getRowSize() == 2);
            assert(a.getColSize() == 2);

            int row0[] = {1, 2};
            a.writeRow(0, row0);
            int row1[] = {3, 4};
            a.writeRow(1, row1);
            assert(a.getRowSize() == 2);

            int row3[] = {5, 6};
            a.pushRow(row3);
            assert(a(2, 0) == 5);
            assert(a.getRowSize() == 3);
            a.pushRow(row3);
            assert(a(2, 1) == 6);
            assert(a(3, 0) == 5);

            a.writeRow(10, row3);
            assert(a.getRowSize() == 11);
            assert(a(10, 0) == 5);
            assert(a(3, 0) == 5);
        }

        void test_swap_rows() {
            utils::SMatrix<int> a(4, 5);

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

        void test_comparison() {
            utils::SMatrix<int> a(2, 2);
            utils::SMatrix<int> b(1, 2);

            assert(a != b);

            utils::SMatrix<int> c(2, 2);

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
            utils::SMatrix<int> a(2, 2);
            utils::SMatrix<int> c(2, 2);

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

        void test_get_row() {
            utils::SMatrix<int> a(2, 4);


            int row0[] = {1, 1, 1, 1};
            a.writeRow(0, row0);
            a.writeRow(1, row0);

            int* row = a.getRow(0);
            for (size_t i=0; i<a.getColSize(); ++i) {
                assert(row[i]==1);
            }
        }

        void test_create_clone() {
            utils::SMatrix<int> a(4, 5);

            int row0[] = {1, 2, 3, 4, 5};
            a.writeRow(0, row0);
            int row1[] = {6, 7, 8, 9, 10};
            a.writeRow(1, row1);
            int row2[] = {11, 12, 13, 14, 15};
            a.writeRow(2, row2);
            int row3[] = {16, 17, 18, 19, 20};
            a.writeRow(3, row3);

            utils::SMatrix<int>* cl1 = a.createClone(1, 1, 2, 3);
            assert(cl1->getRowSize() == 2);
            assert(cl1->getColSize() == 3);
            assert((*cl1)(0,0) == 7);
            assert((*cl1)(0,2) == 9);
            assert((*cl1)(1,0) == 12);
            assert((*cl1)(1,2) == 14);

            utils::SMatrix<int>* cl2 = a.createClone(1, 3);
            assert(cl2->getRowSize() == 3);
            assert(cl2->getColSize() == 2);
            assert((*cl2)(0,0) == 9);

            utils::SMatrix<int>* cl3 = a.createClone(); // full copy
            assert(cl3->getRowSize() == 4);
            assert(cl3->getColSize() == 5);
            assert((*cl3)(0,0) == 1);

            delete cl1;
            delete cl2;
            delete cl3;
        }

        void test_bit_set() {
            utils::IntBitSet<int> a;

            a.add(200);
            a.print();
        }

        void sMatrix_test() {
            suite("SMatrix");
            test(push_row);
            test(swap_rows);
            test(comparison);
            test(comparison_with_error);
            test(create_clone);
            test(bit_set);
        }
    }
}



