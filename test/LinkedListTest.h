#include <iostream>
#include <vector>
#include <list>
#include <assert.h>

#include "test.h"
#include "utils/LinkedList.h"

void linked_list_test();

struct Foo {
	char ch;
	int id;

//	Foo(const Foo& f) :
//			ch(0), id(0) {
//		std::cout << "Foo: Run copy constructor" << std::endl;
//	}

	Foo(char _ch, int _id) :
			ch(_ch), id(_id) {
	}
};

class FooC {
	bool bb;
};

