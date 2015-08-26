#include <iostream>
#include <string>
#include "foo/foo.h"
#include "Sales_item.h"

//using namespace std;
using std::wstring;

int main(int argc, char** argv) {
	int val1 = 6;
	int val2 = 8;
	int val4 = 10;

	int* val3 = nullptr;

	std::cout << val1 << std::endl;
	std::cout << val2 << std::endl;
	std::cout << val4 << std::endl;

	hello_world(val1, &val2, val4);

	std::cout << val1 << std::endl;
	std::cout << val2 << std::endl;
	std::cout << val3 << std::endl;
	std::cout << val4 << std::endl;

	const wchar_t* st = L"Цена бутылки вина";
	wstring st2 = L"Цена бутылки вина";
	std::cout << st << std::endl;
	int len = stringLength(st);

	std::cout << len << std::endl;
    //std::cout << st << std::endl;

    std::cout << st2.size() << std::endl;

    const int val6 = 7;
    const int* pVal6 = &val6;
    std::cout << *pVal6 << std::endl;

    int val7 = 8;
    pVal6 = &val7;

    const int &ir = 1024;
    double dval = 3.14;
    const int &ir2 = dval;

    const double mw = 9.6;
    const double* pc = 0;
    pc = &mw;
    // *pc = 3.0; - нельзя ошибка т.к. pc - это указатель наобъект const double

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
