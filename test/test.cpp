
#include <iostream>

#include "test.h"
#include "LinkedListTest.h"
#include "CArrayListTest.h"
#include "CMatrixTest.h"
//#include "utils/String.h"
#include "myxcb.h"


//using namespace std;
//using std::wstring;

//static void test_size_string() {
//	utils::String buf = "123456";
//	assert(buf.size() == 6);
//}

int main(int argc, char** argv) {

//	suite("String");
//	test(size_string);

//	suite("LinkedList");
//	test(linked_list_create);
//	test(linked_list_iterator);

	linkedList_test();
	cArrayList_test();
	cMatrix_test();

	suite("Chart");
//	test(chart);

	return 0;

}
