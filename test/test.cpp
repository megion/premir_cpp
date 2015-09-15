#include <iostream>
#include <string>
#include <assert.h>
#include "test.h"
#include "utils/String.h"

//using namespace std;
//using std::wstring;

static void
test_size_string() {
	utils::String buf = "123456";
	assert(buf.size()==6);
}

int main(int argc, char** argv) {

	suite("String");
	test(size_string);



//    buf = "Цена моей бутылки вина";
//    std::cout << buf << std::endl;
//    std::cout << buf.size() << std::endl;
	return 0;

}
