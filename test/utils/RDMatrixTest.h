#ifndef TEST_UTILS_RDMATRIX_TEST_H
#define TEST_UTILS_RDMATRIX_TEST_H

#include <iostream>

#include "test.h"
#include "utils/RDMatrix.h"

namespace test {
    namespace utils_rdmatrix {

		class WithDistruction {
			public:
				WithDistruction(): value(0) {
					std::cout << "Call constructor WithDistruction class" << std::endl;
				}
				~WithDistruction() {
					std::cout << "Call destuction ~WithDistruction class" << std::endl;
				}
				WithDistruction(WithDistruction&& v) {
					std::cout << "Call move constructor WithDistruction class" << std::endl;
				}

				WithDistruction(const WithDistruction& v) {
					std::cout << "Call move constructor WithDistruction class" << std::endl;
				}

				WithDistruction &operator=(WithDistruction&& other) {
					std::cout << "Call move = operator "<< std::endl;
					return *this;
				} 

				WithDistruction &operator=(const WithDistruction& other) {
					std::cout << "Call copy = operator "<< std::endl;
					return *this;
				} 
			private:
				int value;
		};
        void rDMatrix_test();
    }
}

#endif

