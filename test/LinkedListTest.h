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

	Foo(const Foo& f) :
			ch(f.ch), id(f.id) {
//		std::cout << "Foo: Run copy constructor" << std::endl;
	}

	Foo(Foo&& f) :
			ch(f.ch), id(f.id) {
//		std::cout << "Foo: Run replace constructor" << std::endl;
	}

	Foo(char _ch, int _id) :
			ch(_ch), id(_id) {
//		std::cout << "Foo: Run init constructor" << std::endl;
	}
};

class FooC {
	bool bb;
};

