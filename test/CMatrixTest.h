#ifndef TEST_C_MATRIX_TEST_H_
#define TEST_C_MATRIX_TEST_H_

#include "../src/utils/CMatrix.h"

#include <iostream>
#include <assert.h>

#include "test.h"

void cMatrix_test();

struct Foo3 {
	char ch;
	int id;

	Foo3(const Foo3& f) :
			ch(f.ch), id(f.id) {
		std::cout << "Foo2: Run copy constructor" << std::endl;
	}

	Foo3(Foo3&& f) :
			ch(f.ch), id(f.id) {
		std::cout << "Foo2: Run replace constructor" << std::endl;
	}

	Foo3(char _ch, int _id) :
			ch(_ch), id(_id) {
		std::cout << "Foo2: Run init constructor" << std::endl;
	}
};

#endif

