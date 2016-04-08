#ifndef TEST_UTILS_HASHMAP_TEST_H
#define TEST_UTILS_HASHMAP_TEST_H

#include <iostream>
#include <cstdio>

#include "test.h"
#include "utils/HashMap.h"
#include "utils/hash/StringHash.h"

namespace test {
    namespace utils_hashMap {

        void hashMap_test();

        struct MyValue {
            int count;
            int value2;
        };

        struct MyStringKey {
            char label[256];
            size_t length;
            void setLabel(const char* val) {
                size_t len = std::strlen(val);
                std::memcpy(label, val, len);
                label[len]='\0';
                length = len;
            }

            // compare function for hash map
            bool operator==(const MyStringKey &other) const {
                if(length!=other.length) {
                    return false;
                }
                return std::strncmp(label, other.label, length)==0;
            }
            bool operator!=(const MyStringKey &other) const {
                return !((*this) == other);
            }
        };

        class MyStringKeyHash : public utils::hash::HashEngine<MyStringKey> {
        public:
            MyStringKeyHash() : stringHash(utils::hash::StringHash<const char*>()) {
            }
            size_t hashCode(const MyStringKey &value) const {
                return stringHash.hashCode(value.label);
            }
        private:
            utils::hash::StringHash<const char*> stringHash;
        };

        class MyValueUpdater : public utils::ValueUpdater<MyValue> {

            void update(MyValue& oldValue, const MyValue& newValue, const size_t valueSizeof) const {
                oldValue.count++;
            }

        };
    }
}

#endif

