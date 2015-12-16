#ifndef TEST_UTILS_C_MATRIX_TEST_H_
#define TEST_UTILS_C_MATRIX_TEST_H_

#include <iostream>

#include "test.h"
#include "utils/CMatrix.h"

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

