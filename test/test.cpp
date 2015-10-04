#include <iostream>
#include <string>
#include <assert.h>
#include "test.h"
#include "utils/String.h"
#include "linked-list-test.h"
//#include "utils/LinkedList.h"
//#include "utils/LinkedListIterator.h"

//using namespace std;
//using std::wstring;

static void test_size_string() {
	utils::String buf = "123456";
	assert(buf.size() == 6);
}

//static void test_linked_list_create() {
//	utils::LinkedList<int> list;
//	list.push(1);
//	list.push(2);
//	assert(list.size() == 2);
//
//	utils::LinkedList<int> list2;
//	list2.push(3);
//	assert(list2.size() == 1);
//
//	list.concat(list2);
////	std::cout<<"size:"<< list.size() << std::endl;
//	assert(list.size() == 3);
//
//	utils::LinkedList<int> list3(list);
//	assert(list3.size() == 3);
//}
//
//static void test_linked_list_iterator() {
//	utils::LinkedList<int> list;
//	list.push(10);
//	list.push(20);
//	list.push(30);
//	int i = 0;
//	// using while cycle
//	utils::LinkedListIterator<int> iter = list.iterator();
//	while(iter.hasNext()) {
//		i++;
//		utils::LinkedListItem<int>* item = iter.next();
////		std::cout<<"item" << i << ": " << item->getValue() << std::endl;
//	}
//	assert(i == 3);
//
//	// using for cycle
//	utils::LinkedListIterator<int> iter2 = list.iterator();
//	i = 0;
//	for (;iter2.hasNext();) {
//		i++;
//		utils::LinkedListItem<int>* item = iter2.next();
////		std::cout<<"item2" << i << ": " << item->getValue() << std::endl;
//	}
//	assert(i == 3);
//}

int main(int argc, char** argv) {

//	suite("String");
//	test(size_string);

//	suite("LinkedList");
//	test(linked_list_create);
//	test(linked_list_iterator);

	linked_list();

	return 0;

}
