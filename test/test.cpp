#include <iostream>
#include <string>
#include <assert.h>
#include <LinkedListTest.h>
#include "test.h"
#include "utils/String.h"
#include "myxcb.h"
//#include "utils/LinkedList.h"
//#include "utils/LinkedListIterator.h"

//using namespace std;
//using std::wstring;

static void test_size_string() {
	utils::String buf = "123456";
	assert(buf.size() == 6);
}

int main(int argc, char** argv) {

	suite("String");
	test(size_string);

//	suite("LinkedList");
//	test(linked_list_create);
//	test(linked_list_iterator);

	linked_list_test();

//	xcbtest();
//	xcbtest2();

	suite("Chart");
	test(chart);

	return 0;

}
