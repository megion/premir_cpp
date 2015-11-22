#include <LinkedListTest.h>

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

void test_linked_list_iterator() {
	utils::LinkedList<int> list;
	int aa = 2;
	list.push(aa);
	list.push(aa);
	list.push(aa);
	int i = 0;
	// using while cycle
	utils::LinkedList<int>::Iterator iter = list.iterator();
	while (iter.hasNext()) {
		i++;
		iter.next();
//		std::cout<<"item" << i << ": " << item->getValue() << std::endl;
	}
	assert(i == 3);

	// using for cycle
	utils::LinkedList<int>::Iterator iter2 = list.iterator();
	i = 0;
	for (; iter2.hasNext();) {
		i++;
		utils::LinkedList<int>::Entry* entry = iter2.next();
		int v2 = entry->getValue();
		std::cout << "item" << i << ": " << v2 << std::endl;
	}

	assert(i == 3);

	utils::LinkedList<int> list3;
	list3.push(aa);
	list3 = list;
	assert(list3.size() == 3);
}

void test_vector() {
	std::vector<int> list1;
	list1.push_back(10);
	list1.push_back(20);

	std::vector<Foo*> list2;
	Foo v0 = Foo('1', -1);
	list2.push_back(&v0);
	for (int i = 0; i < 100; ++i) {
		Foo v = Foo('1', i);
		std::cout << "&v2: " << &v << std::endl;
		list2.push_back(&v);
	}

	assert(list1.size() == 2);

//	int i = 0;
	v0.id = -10;

	for (std::vector<Foo*>::iterator iter = list2.begin(); iter != list2.end();
			++iter) {
		std::cout << "(*iter).id: " << (*iter)->id << std::endl;
//		i++;
	}

//	assert(i == 2);

	std::cout << "size char = " << sizeof(char) << std::endl;
	std::cout << "size int = " << sizeof(int) << std::endl;
	std::cout << "size Foo = " << sizeof(Foo) << std::endl;
	std::cout << "size FooC = " << sizeof(FooC) << std::endl;

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
	test(concat_and_copy);
	test(linked_list_iterator);

//	suite("Vector");
//	test(vector);
//
//	suite("List");
//	test(list);
}

