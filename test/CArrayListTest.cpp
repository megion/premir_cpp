#include <CArrayListTest.h>

void test_push() {
	utils::CArrayList<Foo2> list;
	Foo2 f = Foo2('1', 2);
	list.push(f);
	assert(list.size() == 1);
	assert(list[0].ch == '1');

//	for (Foo2& ff : list) {
//		std::cout << "value ff:" << ff.ch << std::endl;
//	}

	list.push(Foo2('3', 4));
	assert(list.size() == 2);
	assert(list[1].ch == '3');

	Foo2 farr[] = { { '5', 6 }, { '7', 8 } };
	list.push(farr, 2);
	assert(list.size() == 4);
	assert(list[3].ch == '7');

	for (int i = 0; i < 20; ++i) {
		Foo2 ff4 = Foo2('0', i + 9);
		list.push(ff4);
	}

	assert(list.size() == 24);
	assert(list[list.size()-1].id == 28);

}

void cArrayList_test() {
	suite("CArrayList");
	test(push);
}
