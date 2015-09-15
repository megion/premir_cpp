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

	return 0;
}
