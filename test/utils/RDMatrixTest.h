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
			private:
				int value;
		};
        void rDMatrix_test();
    }
}

#endif

