#include <iostream>
#include "foo/foo.h"
#include "Sales_item.h"

int main(int argc, char** argv) {
	//hello_world();
	int sum = 0;
	int i;
	while (std::cin >> i) {
		sum += i;
		std::cout << sum << std::endl;
	}
	std::cout << sum << std::endl;
	return 0;
//    Sales_item item1, item2;
//    std::cin >> item1 >> item2;
//    if (item1.isbn() == item2.isbn())
//    {
//        std::cout << item1 + item2 << std::endl;
//        return 0;
//    }
//    else
//    {
//        std::cerr << "Data must refer to same ISBN." << std::endl;
//        return -1;
//    }
}
