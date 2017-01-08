#ifndef TEST_UTILS_C_ARRAY_LIST_TEST_H_
#define TEST_UTILS_C_ARRAY_LIST_TEST_H_

#include <iostream>
#include <assert.h>

#include "utils/CArrayList.h"

#include "test.h"

namespace test {
	namespace utils_carray_list {
		void cArrayList_test();

		struct Foo2 {
			char ch;
			int id;

			Foo2(const Foo2& f) : ch(f.ch), id(f.id) {
				std::cout << "Foo2: Run copy constructor" << std::endl;
			}

			Foo2(Foo2&& f) : ch(f.ch), id(f.id) {
				std::cout << "Foo2: Run replace constructor" << std::endl;
			}

			Foo2(char _ch, int _id) : ch(_ch), id(_id) {
				//		std::cout << "Foo2: Run init constructor" << std::endl;
			}
		};
	}
}


#endif

