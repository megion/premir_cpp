#include "HashMapTest.h"

namespace test {
	namespace utils_hashMap {

		void test_push_value() {
			utils::hash::CharHash shash;
			shash.setIndexSize(6);
			utils::HashMap<char, int> a(&shash);

			a.pushValue('K', 4);
			assert(a.getValue('K') != nullptr);
			assert((*a.getValue('K')) == 4);

			a.pushValue('A', 7);
			assert(a.getValue('K') != nullptr);
			assert((*a.getValue('A')) == 7);

			a.pushValue('K', 5);
			assert((*a.getValue('K')) == 5);

			a.pushValue('P', 55);
			assert((*a.getValue('K')) == 5);
			assert((*a.getValue('P')) == 55);
			assert(a.getValue('M') == nullptr);

			MyStringKeyHash strHash;
			strHash.setIndexSize(6);
			utils::HashMap<MyStringKey, int> b(&strHash);

			MyStringKey mystr;
			mystr.setLabel("my string");
			b.pushValue(mystr, 4);
			assert(b.getValue(mystr) != nullptr);
			assert((*b.getValue(mystr)) == 4);

			b.pushValue(mystr, 42);
			assert((*b.getValue(mystr)) == 42);
		}

		void test_push_or_update_value() {
			MyStringKeyHash strHash;
			utils::HashMap<MyStringKey, MyValue> b(&strHash);
			MyValueUpdater updater;
			MyValue v1 = { 1, 11 };

			MyStringKey mykey;
			mykey.setLabel("my key1");
			b.pushValue(mykey, v1, &updater);
			MyValue v2 = { 1, 22 };
			b.pushValue(mykey, v2, &updater);
			MyValue v3 = { 1, 33 };
			b.pushValue(mykey, v3, &updater);
			assert((*b.getValue(mykey)).count == 3);
			assert((*b.getValue(mykey)).value2 == 11);

			MyStringKey mykey2;
			mykey2.setLabel("my key2");
			b.pushValue(mykey2, v1, &updater);
			assert((*b.getValue(mykey)).count == 3);
			assert((*b.getValue(mykey2)).count == 1);
		}

		void hashMap_test() {
			suite("HashMap");
			mytest(push_value);
			mytest(push_or_update_value);
		}
	}
}

