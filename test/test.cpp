
#include <iostream>

//#include "test.h"
#include "utils/LinkedListTest.h"
#include "utils/CArrayListTest.h"
#include "utils/CMatrixTest.h"
//#include "ml/DigitalFilterTest.h"

int main(int argc, char** argv) {

//	suite("String");
//	test(size_string);

//	suite("LinkedList");
//	test(linked_list_create);
//	test(linked_list_iterator);

	linkedList_test();
	cArrayList_test();
	cMatrix_test();
//	digitalFilter1_test();

//	test(chart);

	return 0;

}
