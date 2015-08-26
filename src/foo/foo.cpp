#include <iostream>
#include <string>
#include "foo.h"

//using namespace std::string;
using std::string;


void hello_world(int& refVal, int* pVal, int rVal) {
	// change reference value
	refVal = 7;
	// change pointer value
	*pVal = 9;

	rVal = 12;
	std::cout << "Hello, World5!" << std::endl;
}

int stringLength(const wchar_t* st) {
	int cnt = 0;
	const wchar_t* p = st;
	if (st) {
		while (*p) {
			//std::cout << p << std::endl;
			++cnt;
			p++;
		}
	}
	return cnt;
}

