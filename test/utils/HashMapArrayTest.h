#ifndef TEST_UTILS_HASHMAPARRAY_TEST_H
#define TEST_UTILS_HASHMAPARRAY_TEST_H

#include <iostream>

#include "test.h"
#include "utils/HashMapArray.h"
#include "utils/hash/StringHash.h"

namespace test {
    namespace utils_hashMapArray {

        void hashMapArray_test();

        struct MyValue {
            int count;
            int value2;
        };

        class MyValueUpdater : public utils::ValueUpdater<MyValue> {

            void update(MyValue& oldValue, const MyValue& newValue, const size_t valueSizeof) const {
                oldValue.count++;
            }

        };
    }
}

#endif

