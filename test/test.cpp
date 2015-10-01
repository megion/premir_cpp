#include <iostream>
#include <string>
#include <assert.h>
#include "test.h"
#include "utils/String.h"
#include "utils/LinkedList.h"

//using namespace std;
//using std::wstring;

static void test_size_string() {
	utils::String buf = "123456";
	assert(buf.size() == 6);
}

static void test_linked_list_create() {
	utils::LinkedList<int> list;
	list.push(1);
	list.push(2);
	assert(list.size() == 2);

	const utils::LinkedList<int> list2;
	list2.push(3);

	list.concat(list2);
	assert(list.size() == 3);
}

int main(int argc, char** argv) {

	suite("String");
	test(size_string);

	suite("LinkedList");
	test(linked_list_create);

//    buf = "Цена моей бутылки вина";
//    std::cout << buf << std::endl;
//    std::cout << buf.size() << std::endl;
	return 0;

}
