#include <iostream>
#include <vector>
#include <list>
#include <assert.h>

#include "test.h"
#include "utils/LinkedList.h"

void linked_list_test();

struct Foo {
	char ch;
	char ch1;
	short id;
	int value;

	Foo(const Foo& f) :
			ch(0), ch1(0), id(0), value(0) {

		std::cout << "Foo: Run copy constructor" << std::endl;
	}

	Foo(char _ch, char _ch1, short _id, int _value) :
			ch(_ch), ch1(_ch1), id(_id), value(_value) {
	}
};

class FooC {
	bool bb;
};

