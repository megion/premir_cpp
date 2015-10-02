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

	utils::LinkedList<int> list2;
	list2.push(3);

	list.concat(list2);
//	assert(list.size() == 3);
}

struct mstruct {
	int v1;
	int v2;
	mstruct(int _v1, int _v2) {
		v1 = _v1;
		v2 = _v2;
	}

	int getV1() const {
		return v1;
	}
};

void mytest(const mstruct* msPtr) {
	int v1 = msPtr->getV1();
	std::cout<<"v1:"<< v1 << std::endl;
}

int main(int argc, char** argv) {

	suite("String");
	test(size_string);

	suite("LinkedList");
	test(linked_list_create);

	const mstruct ms(5,6);

	mytest(&ms);

//    buf = "Цена моей бутылки вина";
//    std::cout << buf << std::endl;
//    std::cout << buf.size() << std::endl;
	return 0;

}
