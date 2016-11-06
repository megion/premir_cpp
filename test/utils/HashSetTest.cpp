#include <HashSetTest.h>

namespace test {
	namespace utils_hash_set {

		void test_push_value() {
			utils::hash::CharHash shash;
			shash.setIndexSize(6);
			utils::HashSet<char> a(&shash);

			assert(a.contains('K') == false);
			assert(a.push('K') == true);
			assert(a.push('K') == false); // is not pushed - already exist
			assert(a.contains('K') == true);

			assert(a.contains('A') == false);
			assert(a.push('A') == true);
			assert(a.push('A') == false);
			assert(a.contains('A') == true);
			assert(a.contains('K') == true);

			utils_hash_map::MyStringKeyHash strHash;
			strHash.setIndexSize(6);
			utils::HashSet<utils_hash_map::MyStringKey> b(&strHash);

			utils_hash_map::MyStringKey mystr;
			mystr.setLabel("my string");
			assert(b.push(mystr) == true);
			assert(b.push(mystr) == false);
			assert(b.contains(mystr) == true);
		}

		void test_push_or_update_value() {
			utils_hash_map::MyStringKeyHash strHash;
			utils::HashSet<utils_hash_map::MyStringKey> b(&strHash);
			utils_hash_map::MyKeyUpdater updater;

			utils_hash_map::MyStringKey mykey1;
			mykey1.setLabel("my key1");
			mykey1.count2 = 1;
			b.push(mykey1, &updater);

			utils_hash_map::MyStringKey mykey2;
			mykey2.setLabel("my key1");
			mykey2.count2 = 1;
			b.push(mykey2, &updater);

			utils_hash_map::MyStringKey mykey3;
			mykey3.setLabel("my key1");
			mykey3.count2 = 1;
			b.push(mykey3, &updater);

			assert(mykey1.count2 == 3);

			utils_hash_map::MyStringKey mykey4;
			mykey4.setLabel("my key2");
			mykey4.count2 = 1;
			b.push(mykey4, &updater);

			assert(mykey1.count2 == 3);
			assert(mykey4.count2 == 1);
		}

		void hashSet_test() {
			suite("HashSet");
			mytest(push_value);
			mytest(push_or_update_value);
		}
	}
}

