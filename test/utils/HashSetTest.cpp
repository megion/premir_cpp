#include <HashSetTest.h>

namespace test {
	namespace utils_hash_set {

		void test_push_value() {
			utils::hash::CharHash shash;
			shash.setIndexSize(6);
			utils::HashSet<char> a(&shash);

			assert(a.contains('K') == false);
			assert(a.push('K')==true);
			assert(a.push('K')==false); // is not pushed - already exist
			assert(a.contains('K') == true);

			assert(a.contains('A') == false);
			assert(a.push('A')==true);
			assert(a.push('A')==false);
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

		void hashSet_test() {
			suite("HashSet");
			mytest(push_value);
		}
	}
}

