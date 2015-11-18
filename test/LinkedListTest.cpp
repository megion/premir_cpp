#include <LinkedListTest.h>

void test_linked_list_create() {
	utils::LinkedList<int> list;
	int a = 1;
	list.push(&a);
	list.push(&a);
	assert(list.size() == 2);

	utils::LinkedList<int> list2;
	list2.push(&a);
	assert(list2.size() == 1);

	list.concat(list2);
//	std::cout<<"size:"<< list.size() << std::endl;
	assert(list.size() == 3);

	utils::LinkedList<int> list3(list);
	assert(list3.size() == 3);
}

void test_linked_list_iterator() {
	utils::LinkedList<int> list;
	int aa = 2;
	list.push(&aa);
	list.push(&aa);
	list.push(&aa);
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
		int* v2 = entry->getValue();
		std::cout << "item" << i << ": " << *v2 << std::endl;
	}

	assert(i == 3);

	utils::LinkedList<int> list3;
	list3.push(&aa);
	list3 = list;
	assert(list3.size() == 3);
}

void test_push_values() {
	utils::LinkedList<Foo> list;

	Foo f1 = Foo('1', '2', 3, 4);
	Foo* fp1 = &f1;
	std::cout << "fp1: " << fp1 << std::endl;
	list.push(fp1);
	Foo f2 = Foo('5', '6', 7, 8);
	Foo* fp2 = &f2;
	std::cout << "fp2: " << fp2 << std::endl;
	list.push(fp2);


	utils::LinkedList<Foo>::Iterator iter = list.iterator();
	int i = 0;
	while (iter.hasNext()) {
		utils::LinkedList<Foo>::Entry* e = iter.next();
		Foo* fv = e->getValue();
		std::cout << "fv: " << fv->value << std::endl;
		std::cout << "fp new" << i << ": " << fv << std::endl;
		if (i == 0) {
			assert((fv) == (fp1));
		} else if (i == 1) {
			assert((fv) == (fp2));
		}
		i++;

	}
}

void test_vector() {
	std::vector<int> list1;
	list1.push_back(10);
	list1.push_back(20);

	assert(list1.size() == 2);

	int i = 0;
	for (std::vector<int>::iterator iter = list1.begin(); iter != list1.end();
			++iter) {

		i++;
	}

	assert(i == 2);

	std::cout << "size char = " << sizeof(char) << std::endl;
	std::cout << "size int = " << sizeof(int) << std::endl;
	std::cout << "size Foo = " << sizeof(Foo) << std::endl;
	std::cout << "size FooC = " << sizeof(FooC) << std::endl;

}

void linked_list_test() {
	suite("LinkedList");
	test(linked_list_create);
	test(linked_list_iterator);
	test(push_values);

	suite("Vector");
	test(vector);
}

