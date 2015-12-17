#include <iostream>

#include "utils/LinkedListTest.h"
#include "utils/CArrayListTest.h"
#include "utils/CMatrixTest.h"
#include "ml/DigitalFilterTest.h"

int main(int argc, char** argv) {

	linkedList_test();
	cArrayList_test();
	cMatrix_test();
	digitalFilter_test();

	return 0;

}
