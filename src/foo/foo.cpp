#include <iostream>
#include "foo.h"
void hello_world(int& refVal, int* pVal) {
	// change reference value
	refVal = 7;
	// change pointer value
	*pVal = 9;
	std::cout << "Hello, World5!" << std::endl;
}

