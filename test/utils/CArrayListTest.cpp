#include "CArrayListTest.h"

namespace test {
	namespace utils_carray_list {
		

		// initialize static variable
		common::CallInfo Foo2::callInfo = common::CallInfo();

		void test_push() {
			Foo2::callInfo.reset();
			utils::CArrayList<Foo2> list;
			Foo2 f = Foo2('1', 2);
			list.push(f);
			assert(list.size() == 1);
			assert(list[0].ch == '1');

			list.push(Foo2('3', 4));
			assert(list.size() == 2);
			assert(list[1].ch == '3');

			Foo2 farr[] = {{'5', 6},
				{'7', 8}};
			list.push(farr, 2);
			assert(list.size() == 4);
			assert(list[3].ch == '7');

			for (int i = 0; i < 20; ++i) {
				Foo2 ff4 = Foo2('0', i + 9);
				list.push(ff4);
			}

			assert(list.size() == 24);
			assert(list[list.size() - 1].id == 28);

		}

		void test_copy_constructor() {
			utils::CArrayList<Foo2> list;
			list.push(Foo2('1', 2));
			list.push(Foo2('3', 4));

			utils::CArrayList<Foo2> copyList(list);
			assert(copyList.size() == 2);

			// change values for first list
			for (Foo2 &ff : list) {
				ff.id = 8;
			}

			assert(list[0].id == 8); // check changes
			assert(copyList[0].id == 2); // check no changes in copy
		}

		void test_capacity_increase() {
			utils::CArrayList<int> list(3, 5);
			assert(list.getCapacity()==3);
			assert(list.size()==0);

			int arr[] = {0,1,2,3,4,5};
			list.push(arr, 6);
			assert(list.getCapacity()==8);
			assert(list.size()==6);

			list.push(arr, 3);
			assert(list.getCapacity()==13);
			assert(list.size()==9);

			list.push(arr, 4);
			std::cout << "list.getCapacity " << list.getCapacity() << std::endl;
			std::cout << "list.size " << list.size() << std::endl;
			assert(list.getCapacity()==13);
			assert(list.size()==13);
		}

		void cArrayList_test() {
			suite("CArrayList");
			mytest(push);
			mytest(copy_constructor);
			mytest(capacity_increase);
		}
	}
}

