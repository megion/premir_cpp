#include "RDMatrixTest.h"

namespace test {
    namespace utils_rdmatrix {

		// initialize static variable
		common::CallInfo MyArray::callInfo = common::CallInfo();

        void test_push_row() {
            utils::RDMatrix<double, int> a;

            int points0[] = {1, 2};
            utils::RDMatrix<double, int>::Row row0;
            row0.points = points0;
            row0.pointSize = 2;
            a.writeRow(0, row0);

            int points1[] = {3, 4};
            a.writeRow(1, points1, 2);

            // число строк = 2
            assert(a.getRowSize() == 2);

            int points3[] = {5, 6, 7};
            utils::RDMatrix<double, int>::Row row3;
            row3.points = points3;
            row3.pointSize = 3;
            a.pushRow(row3);

            assert(a(2, 0) == 5);
            assert(a(2, 2) == 7);
            assert(a.getRowSize() == 3);

            a.pushRow(points3, 3);
            assert(a(2, 1) == 6);
            assert(a(3, 0) == 5);
            assert(a(3, 2) == 7);

            a.writeRow(10, row3);
            assert(a.getRowSize() == 11);
            assert(a(10, 0) == 5);
            assert(a(3, 0) == 5);
            assert(a(3, 2) == 7);

            a.writeRow(16, points3, 3);
            assert(a.getRowSize() == 17);
            assert(a(16, 0) == 5);
            assert(a(3, 2) == 7);

            int points4[] = {5, 6, 7, 8};
            // replace exist
            a.writeRow(16, points4, 4);
            assert(a.getRowSize() == 17);
            assert(a(16, 0) == 5);
            assert(a(16, 3) == 8);
        }

        void test_comparison() {
            utils::RDMatrix<double, int> a;
            utils::RDMatrix<double, int> b;

            assert(a == b);

            utils::RDMatrix<double, int> c;

            int points0[] = {1, 2};
            utils::RDMatrix<double, int>::Row row0;
            row0.points = points0;
            row0.data = 2;
            row0.pointSize = 2;
            a.writeRow(0, row0);
            c.writeRow(0, row0);

            int points1[] = {5, 6};
            utils::RDMatrix<double, int>::Row row1;
            row1.points = points1;
            row1.data = 4;
            row1.pointSize = 2;
            a.writeRow(1, row1);
            c.writeRow(1, row1);

            assert(a == c);

            c(1,1) = 8;
            assert(a != c);
        }

        void test_comparison_with_error() {
            utils::RDMatrix<double, int> a;
            utils::RDMatrix<double, int> c;

            int points0[] = {1, 2};
            utils::RDMatrix<double, int>::Row row0;
            row0.points = points0;
            row0.data = 2;
            row0.pointSize = 2;
            a.writeRow(0, row0);
            c.writeRow(0, row0);

            int points1[] = {5, 6};
            utils::RDMatrix<double, int>::Row row1;
            row1.points = points1;
            row1.data = 4;
            row1.pointSize = 2;
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
            utils::RDMatrix<double, int> a;

            int row0[] = {1, 1, 1, 1};
            a.writeRow(0, row0, 4);
            a.writeRow(1, row0, 4);

            for (size_t r=0; r<a.getRowSize(); ++r) {
                utils::RDMatrix<double, int>::Row& row = a[r];
                for (size_t i=0; i<row.pointSize; ++i) {
                    assert(row.points[i]==1);
                }
            }
        }

        void test_append_values() {
            utils::RDMatrix<double, int> a;

            int row0[] = {1, 1, 1, 1};
            a.writeRow(0, row0, 4);
            a.writeRow(1, row0, 4);

            int vals[] = {2, 3, 4, 5};
            a.writeToEndRow(0, vals, 4);

            assert(a[0].pointSize==8);
            assert(a(0, 7)==5);
            assert(a.getRowSize()==2);

            a.writeToEndRow(2, vals, 4);
            assert(a.getRowSize()==3);
            assert(a[2].pointSize == 4);
            assert(a(2, 3) == 5);

            int val = 12;
            a.writeToEndRow(0, val);
            assert(a[0].pointSize==9);
            assert(a(0, 8)==12);
            assert(a.getRowSize()==3);
        }

        void test_capacity() {
            utils::RDMatrix<double, int> a(10, 1, 1);
            assert(a.getRowSize()==10);
            int row0[] = {1, 1, 1, 1};
            a.pushRow(row0, 4);
            assert(a.getRowSize()==11);
            a.pushRow(row0, 4);
            assert(a.getRowSize()==12);

            utils::RDMatrix<double, int> b(10, 12, 9);
            assert(b.getRowSize()==10);
            b.pushRow(row0, 4);
            assert(b.getRowSize()==11);
            b.writeToEndRow(9, row0, 4);
            b.writeToEndRow(9, row0, 4);
            b.writeToEndRow(9, row0, 4);

            assert(b[9].pointSize==12);
            assert(b[9].pointCapacity==18);

            int row1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20};

            b.writeToEndRow(9, row1, 20);
            assert(b[9].pointSize==32);
            assert(b[9].pointCapacity==32);

            utils::RDMatrix<double, int> ab(16, 19);
            assert(ab.getRowSize()==16);
            assert(ab[15].pointSize==19);
            assert(ab[15].pointCapacity==19);
        }

		void move_temp_objects(utils::RDMatrix<bool, MyArray>& matrix) {
			MyArray b;
			matrix.pushRow(std::move(b));
		}

        void test_copy_constructor_and_copy_operator() {
			MyArray::callInfo.reset();
			assert(MyArray::callInfo.defaultConstructorCallCount==0);
			assert(MyArray::callInfo.copyConstructorCallCount==0);
			assert(MyArray::callInfo.moveConstructorCallCount==0);
			assert(MyArray::callInfo.copyOperatorCallCount==0);
			assert(MyArray::callInfo.moveOperatorCallCount==0);
			assert(MyArray::callInfo.destructorCallCount==0);

            MyArray a;
            assert(MyArray::callInfo.defaultConstructorCallCount==1);
			assert(MyArray::callInfo.copyConstructorCallCount==0);
			assert(MyArray::callInfo.moveConstructorCallCount==0);
			assert(MyArray::callInfo.copyOperatorCallCount==0);
			assert(MyArray::callInfo.moveOperatorCallCount==0);
			assert(MyArray::callInfo.destructorCallCount==0);

            MyArray b = a; // call copy constructor
            MyArray c(a); // call copy constructor (variant 2)
            assert(MyArray::callInfo.defaultConstructorCallCount==1);
			assert(MyArray::callInfo.copyConstructorCallCount==2);
			assert(MyArray::callInfo.moveConstructorCallCount==0);
			assert(MyArray::callInfo.copyOperatorCallCount==0);
			assert(MyArray::callInfo.moveOperatorCallCount==0);
			assert(MyArray::callInfo.destructorCallCount==0);
            
            MyArray d; // call constructor
            d = c; // call copy assign operator
            assert(MyArray::callInfo.defaultConstructorCallCount==2);
			assert(MyArray::callInfo.copyConstructorCallCount==2);
			assert(MyArray::callInfo.moveConstructorCallCount==0);
			assert(MyArray::callInfo.copyOperatorCallCount==1);
			assert(MyArray::callInfo.moveOperatorCallCount==0);
			assert(MyArray::callInfo.destructorCallCount==0);

            MyArray f = MyArray(std::move(c)); // call move constructor
            assert(MyArray::callInfo.defaultConstructorCallCount==2);
			assert(MyArray::callInfo.copyConstructorCallCount==2);
			assert(MyArray::callInfo.moveConstructorCallCount==1);
			assert(MyArray::callInfo.copyOperatorCallCount==1);
			assert(MyArray::callInfo.moveOperatorCallCount==0);
			assert(MyArray::callInfo.destructorCallCount==0);

            f = std::move(d); // call move assign operator 
            assert(MyArray::callInfo.defaultConstructorCallCount==2);
			assert(MyArray::callInfo.copyConstructorCallCount==2);
			assert(MyArray::callInfo.moveConstructorCallCount==1);
			assert(MyArray::callInfo.copyOperatorCallCount==1);
			assert(MyArray::callInfo.moveOperatorCallCount==1);
			assert(MyArray::callInfo.destructorCallCount==0);
		}

		void test_push_moved_object() {
			MyArray::callInfo.reset();
			assert(MyArray::callInfo.defaultConstructorCallCount==0);
			assert(MyArray::callInfo.copyConstructorCallCount==0);
			assert(MyArray::callInfo.moveConstructorCallCount==0);
			assert(MyArray::callInfo.copyOperatorCallCount==0);
			assert(MyArray::callInfo.moveOperatorCallCount==0);

            utils::RDMatrix<bool, MyArray> a;
		
			MyArray b;
            a.writeRow(1, std::move(b));
			assert(b.getValues()==nullptr);
			
			assert(MyArray::callInfo.defaultConstructorCallCount==1);
			assert(MyArray::callInfo.copyConstructorCallCount==0);
			assert(MyArray::callInfo.moveConstructorCallCount==0);
			assert(MyArray::callInfo.copyOperatorCallCount==0);
			assert(MyArray::callInfo.moveOperatorCallCount==1);

			assert(MyArray::callInfo.destructorCallCount==0);
			assert(MyArray::callInfo.destructorAndClearResourcesCallCount==0);

			move_temp_objects(a);
			move_temp_objects(a);
			move_temp_objects(a);

			assert(MyArray::callInfo.defaultConstructorCallCount==4);
			assert(MyArray::callInfo.copyConstructorCallCount==0);
			assert(MyArray::callInfo.moveConstructorCallCount==0);
			assert(MyArray::callInfo.copyOperatorCallCount==0);
			assert(MyArray::callInfo.moveOperatorCallCount==4);

			assert(MyArray::callInfo.destructorCallCount==3);
			assert(MyArray::callInfo.destructorAndClearResourcesCallCount==0);

			MyArray::callInfo.reset();
			a.removeAll();

			assert(MyArray::callInfo.defaultConstructorCallCount==0);
			assert(MyArray::callInfo.copyConstructorCallCount==0);
			assert(MyArray::callInfo.moveConstructorCallCount==0);
			assert(MyArray::callInfo.copyOperatorCallCount==0);
			assert(MyArray::callInfo.moveOperatorCallCount==0);
			assert(MyArray::callInfo.destructorCallCount==4);
			assert(MyArray::callInfo.destructorAndClearResourcesCallCount==4);

		}

        void rDMatrix_test() {
            suite("RDMatrix");
            mytest(push_row);
            mytest(comparison);
            mytest(comparison_with_error);
            mytest(get_row);
            mytest(append_values);
            mytest(capacity);
			mytest(push_moved_object);
			mytest(copy_constructor_and_copy_operator);

        }
    }
}



