#include <LinkedList.h>
#include <LinkedListTest.h>
#include <iterator>

void test_push_shift_pop_unshift() {
	utils::LinkedList<Foo> list;
	list.push(Foo('1', 2));
	list.push(Foo('3', 4));
	assert(list.size() == 2);
	list.unshift(Foo('5', 6));
	assert(list.size() == 3);

	// iterate by list3
	utils::LinkedList<Foo>::Iterator iter = list.iterator();
	int i = 0;
	while (iter.hasNext()) {
		utils::LinkedList<Foo>::Entry* e = iter.next();
		Foo& fv = e->getValue();
		if (i == 0) {
			assert(fv.ch == '5');
		} else if (i == 1) {
			assert(fv.ch == '1');
		} else if (i == 2) {
			assert(fv.ch == '3');
		}
		i++;
	}
	assert(i == 3);

	// test shift
	utils::LinkedList<Foo>::Entry* newFirst = list.shift();
	assert(newFirst->getValue().ch == '1');
	assert(list.size() == 2);

	iter = list.iterator();
	i = 0;
	while (iter.hasNext()) {
		utils::LinkedList<Foo>::Entry* e = iter.next();
		Foo& fv = e->getValue();
		if (i == 0) {
			assert(fv.ch == '1');
		} else if (i == 1) {
			assert(fv.ch == '3');
		}
		i++;
	}
	assert(i == 2);

	// test pop
	newFirst = list.pop();
	assert(newFirst->getValue().ch == '1');
	assert(list.size() == 1);
	list.unshift(Foo('8', 10));
	assert(list.size() == 2);
	newFirst = list.shift();
	assert(list.size() == 1);
	std::cout << "newFirst->getValue().ch: "<< newFirst->getValue().ch << std::endl;
	assert(newFirst->getValue().ch == '1');

	newFirst = list.pop();
	assert(list.size() == 0);
	assert(newFirst == nullptr);

	// try shift empty list
	newFirst = list.shift();
	assert(list.size() == 0);
	assert(newFirst == nullptr);
}

void test_concat_and_copy() {
	utils::LinkedList<Foo> list;
	list.push(Foo('1', 2));
	list.push(Foo('3', 4));

	utils::LinkedList<Foo> list2;
	list2.push(Foo('5', 6));
	list2.push(Foo('7', 8));

	list.concat(list2);
	assert(list.size() == 4);

	// test copy construct
	utils::LinkedList<Foo> list3(list);
	assert(list3.size() == 4);

	// test assign operator
	utils::LinkedList<Foo> list4;
	list4.push(Foo('9', 10));
	assert(list4.size() == 1);

	list4 = list3;
	assert(list4.size() == 4);

	// test list4 values
	int i = 0;
	utils::LinkedList<Foo>::Iterator iter = list4.iterator();
	while (iter.hasNext()) {
		utils::LinkedList<Foo>::Entry* e = iter.next();
		Foo& fv = e->getValue();
		if (i == 0) {
			assert(fv.ch == '1');
		} else if (i == 1) {
			assert(fv.ch == '3');
		} else if (i == 2) {
			assert(fv.ch == '5');
		} else if (i == 3) {
			assert(fv.ch == '7');
		}
		i++;
	}
	assert(i == 4);
}

void test_index_operator() {
	utils::LinkedList<Foo> list;
	assert(list[0] == nullptr);
	assert(list[1] == nullptr);

	list.push(Foo('1', 2));
	list.push(Foo('3', 4));
	assert(list[2] == nullptr);

	assert(list[0]->getValue().ch == '1');
	assert(list[1]->getValue().ch == '3');
}

void test_push_temp_values() {
	utils::LinkedList<Foo> list;
	for (int i = 0; i < 12; ++i) {
		list.push(Foo('1', i));
	}

	utils::LinkedList<Foo>::Iterator iter = list.iterator();
	while (iter.hasNext()) {
		utils::LinkedList<Foo>::Entry* e = iter.next();
		std::cout << "item" << ": " << e->getValue().id << std::endl;
	}
}

void test_vector() {
	std::list<Foo> list2;
//	std::vector<Foo> list2;
//	Foo v0 = Foo('1', -1);
//	list2.push_back(v0);
	list2.push_back(Foo('1', -1));
	list2.push_back(Foo('2', -2));
	list2.push_back(Foo('3', -3));

	std::list<Foo> list22(std::move(list2));
	std::cout << "list2 size1: " << list2.size() << std::endl;
	std::cout << "list22 size: " << list22.size() << std::endl;
	list2.push_back(Foo('4', -4));
	std::cout << "list2 size2: " << list2.size() << std::endl;

	std::cout << "MY: " << std::endl;

	utils::LinkedList<Foo> list3;
	list3.push(Foo('1', -1));
	list3.push(Foo('2', -2));
	list3.push(Foo('3', -3));
	// delete test
//	utils::LinkedList<Foo> list33;
//	list33 = std::move(list3);
	utils::LinkedList<Foo>::Iterator iter = list3.iterator();
	while (iter.hasNext()) {
		utils::LinkedList<Foo>::Entry* e = iter.next();
		Foo& fv = e->getValue();
		std::cout << "fv.id: " << fv.id << std::endl;

	}
//	for (int i = 0; i < 6; ++i) {
//		Foo v = Foo('1', 20+i);
//		list2.push_back(std::move(v));
//	}

//	v0.id = -10;

//	std::cout << "ITERATION: " << std::endl;
//	for (std::vector<Foo>::iterator iter = list2.begin(); iter != list2.end();
//			++iter) {
//		std::cout << "(*iter).id: " << (*iter).id << std::endl;
////		i++;
//	}

}

void test_list() {
	std::list<Foo> list2;
	Foo v0 = Foo('1', -1);
	list2.push_back(v0);
	for (int i = 0; i < 100; ++i) {
		Foo v = Foo('1', i);
		std::cout << "&v2: " << &v << std::endl;
		list2.push_back(v);
	}

	v0.id = -10;
	for (std::list<Foo>::iterator iter = list2.begin(); iter != list2.end();
			++iter) {
		std::cout << "(*iter).id: " << (*iter).id << std::endl;
	}

}

void linked_list_test() {
	suite("LinkedList");
	test(push_shift_pop_unshift);
	test(index_operator);
//	test(concat_and_copy);
//	test(push_temp_values);

	suite("Vector");
	test(vector);
//
//	suite("List");
//	test(list);
}

