#include "linked-list-test.h"

void test_linked_list_create() {
	utils::LinkedList<int> list;
	list.push(1);
	list.push(2);
	assert(list.size() == 2);

	utils::LinkedList<int> list2;
	list2.push(3);
	assert(list2.size() == 1);

	list.concat(list2);
//	std::cout<<"size:"<< list.size() << std::endl;
	assert(list.size() == 3);

	utils::LinkedList<int> list3(list);
	assert(list3.size() == 3);
}

void test_linked_list_iterator() {
	utils::LinkedList<int> list;
	list.push(10);
	list.push(20);
	list.push(30);
	int i = 0;
	// using while cycle
	utils::LinkedList<int>::Iterator iter = list.iterator();
	while(iter.hasNext()) {
		i++;
		utils::LinkedListItem<int>* item = iter.next();
//		std::cout<<"item" << i << ": " << item->getValue() << std::endl;
	}
	assert(i == 3);

	// using for cycle
	utils::LinkedList<int>::Iterator iter2 = list.iterator();
	i = 0;
	for (;iter2.hasNext();) {
		i++;
		utils::LinkedListItem<int>* item = iter2.next();
//		std::cout<<"item2" << i << ": " << item->getValue() << std::endl;
	}
	assert(i == 3);
}

void test_vector() {
	std::vector<int> list1;
	list1.push_back(10);
	list1.push_back(20);

	assert(list1.size() == 2);

	int i = 0;
	for(std::vector<int>::iterator iter = list1.begin(); iter!=list1.end(); ++iter) {

		i++;
	}

	assert(i == 2);
}

void linked_list() {
	suite("LinkedList");
	test(linked_list_create);
	test(linked_list_iterator);

	suite("Vector");
	test(vector);
}


