#include <iostream>
#include <string>
#include "foo/foo.h"
#include "utils/String.h"
#include "Sales_item.h"

//using namespace std;
using std::wstring;

int main(int argc, char** argv) {
	const wchar_t* st = L"Цена бутылки вина";
	wstring st2 = L"Цена бутылки вина";
	std::cout << st << std::endl;
	int len = stringLength(st);

	std::cout << len << std::endl;


    std::cout << st2.size() << std::endl;

    utils::String buf = "Цена бутылки вина";
    buf = "Цена моей бутылки вина";
    std::cout << buf << std::endl;
    std::cout << buf.size() << std::endl;

//	int sum = 0;
//	int i;
//	while (std::cin >> i) {
//		sum += i;
//		std::cout << sum << std::endl;
//	}
//	std::cout << sum << std::endl;
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
