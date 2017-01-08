#ifndef TEST_UTILS_RDMATRIX_TEST_H
#define TEST_UTILS_RDMATRIX_TEST_H

#include <iostream>

#include "test.h"
#include "utils/RDMatrix.h"

namespace test {
    namespace utils_rdmatrix {

		class MyArray {


			public:

				static common::CallInfo callInfo;

				MyArray(): len(3) {
					values = new int[len];
					//std::cout << "Call constructor MyArray class" << std::endl;
					MyArray::callInfo.defaultConstructorCallCount++;
				}

				~MyArray() {
					MyArray::callInfo.destructorCallCount++;
					//std::cout << "Call destuction ~MyArray class" << std::endl;
					if(values) {
						delete[] values;
						values = nullptr;
						len = 0;
						MyArray::callInfo.destructorAndClearResourcesCallCount++;
						//std::cout << "Call destuction ~MyArray class and clear resources" << std::endl;
					}
				}

				MyArray(MyArray&& v):	
					values(v.values), len(v.len) {
					v.values = nullptr;		
					v.len = 0;
					MyArray::callInfo.moveConstructorCallCount++;
					//std::cout << "Call move constructor MyArray class" << std::endl;
				}

				MyArray(const MyArray& v):len(v.len) {
					values = new int[len];
					for (size_t i=0; i<len; i++) {
						values[i] = v.values[i];// copy object
					}
					MyArray::callInfo.copyConstructorCallCount++;
					//std::cout << "Call move constructor MyArray class" << std::endl;
				}

				MyArray &operator=(const MyArray& other) {
					if (values) {
						delete[] values;
						values = nullptr;
					}
					len = other.len;
					values = new int[len];
					for (size_t i=0; i<len; i++) {
						values[i] = other.values[i];// copy object
					}
					MyArray::callInfo.copyOperatorCallCount++;
					//std::cout << "Call copy = operator "<< std::endl;
					return *this;
				}

				MyArray &operator=(MyArray&& other) {
					values = other.values;
					len = other.len;
					other.values = nullptr;
					other.len = 0;
					MyArray::callInfo.moveOperatorCallCount++;
					//std::cout << "Call move = operator "<< std::endl;
					return *this;
				} 

				int* getValues() const {
					return values;
				}

			private:
				int* values;
				size_t len;

		};

        void rDMatrix_test();
    }
}

#endif

