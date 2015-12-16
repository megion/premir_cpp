#include "LinkedListTest.h"

void test_push_shift_unshift() {
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

//	// test pop
//	newFirst = list.pop();
//	assert(newFirst->getValue().ch == '1');
//	assert(list.size() == 1);
//	list.unshift(Foo('8', 10));
//	assert(list.size() == 2);
//	newFirst = list.shift();
//	assert(list.size() == 1);
//	std::cout << "newFirst->getValue().ch: " << newFirst->getValue().ch
//			<< std::endl;
//	assert(newFirst->getValue().ch == '1');
//
//	newFirst = list.pop();
//	assert(list.size() == 0);
//	assert(newFirst == nullptr);

	// try shift empty list
//	newFirst = list.shift();
//	assert(list.size() == 0);
//	assert(newFirst == nullptr);
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

void test_pop() {
	utils::LinkedList<Foo> list;

	utils::LinkedList<Foo>::Entry* newlast = list.pop();
	assert(newlast == nullptr);
	assert(list.size() == 0);

	list.push(Foo('1', 2));
	list.pop();
	assert(list.size() == 0);

	list.push(Foo('3', 4));
	list.push(Foo('5', 6));
	list.push(Foo('7', 8));
	newlast = list.pop();
	assert(list.size() == 2);
	assert(newlast->getValue().ch == '5');
	assert(list[list.size()-1]->getValue().ch == '5');
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


void linkedList_test() {
	suite("LinkedList");
	test(push_shift_unshift);
	test(index_operator);
	test(pop);
//	test(concat_and_copy);
//	test(push_temp_values);

}

