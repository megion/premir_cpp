#include "HashMapArrayTest.h"

namespace test {
    namespace utils_hash_map_array {

        void test_push_value() {
            utils::hash::HashEngine<char> shash;
            shash.setIndexSize(6);
            utils::HashMapArray<char, int> a(10, &shash);

            a.pushValue(0, 'K', 4);
            assert(a.getValue(0, 'K') != nullptr);
            assert((*a.getValue(0, 'K')) == 4);

            a.pushValue(35, 'A', 7);
            assert(a.getValue(35, 'K') == nullptr);
            assert((*a.getValue(35, 'A')) == 7);

            a.pushValue(0, 'K', 5);
            assert((*a.getValue(0, 'K')) == 5);

            a.pushValue(0, 'P', 55);
            assert((*a.getValue(0, 'K')) == 5);
            assert((*a.getValue(0, 'P')) == 55);
            assert(a.getValue(0, 'M') == nullptr);

            utils_hash_map::MyStringKeyHash strHash;
            strHash.setIndexSize(6);
            utils::HashMapArray<utils_hash_map::MyStringKey, int> b(10, &strHash);

            utils_hash_map::MyStringKey mystr;
            mystr.setLabel("my string");
            b.pushValue(0, mystr, 4);
            assert(b.getValue(0, mystr) != nullptr);
            assert((*b.getValue(0, mystr)) == 4);

            b.pushValue(0, mystr, 42);
            assert((*b.getValue(0, mystr)) == 42);
        }

        void test_push_or_update_value() {
        	utils_hash_map::MyStringKeyHash strHash;
            utils::HashMapArray<utils_hash_map::MyStringKey, utils_hash_map::MyValue> b(10, &strHash);
            utils_hash_map::MyValueUpdater updater;
            utils_hash_map::MyValue v1 = {1, 11};

            utils_hash_map::MyStringKey mykey;
            mykey.setLabel("my key1");
            b.pushValue(0, mykey, v1, &updater);
            utils_hash_map::MyValue v2 = {1, 22};
            b.pushValue(0, mykey, v2, &updater);
            utils_hash_map::MyValue v3 = {1, 33};
            b.pushValue(0, mykey, v3, &updater);
            assert((*b.getValue(0, mykey)).count == 3);
            assert((*b.getValue(0, mykey)).value2 == 11);

            utils_hash_map::MyStringKey mykey2;
            mykey2.setLabel("my key2");
            b.pushValue(0, mykey2, v1, &updater);
            assert((*b.getValue(0, mykey)).count == 3);
            assert((*b.getValue(0, mykey2)).count == 1);
        }

        void hashMapArray_test() {
            suite("HashMapArray");
            mytest(push_value);
            mytest(push_or_update_value);
        }
    }
}



