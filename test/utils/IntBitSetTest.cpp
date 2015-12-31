#include "IntBitSetTest.h"

namespace test {
    namespace utils_intBitSet {

        void test_bit_set() {
            utils::IntBitSet<int> a;

            a.add(0);
            a.add(15);
            a.add(16);
            a.add(17);
            assert(a.contains(15));
            a.remove(15);
            assert(!a.contains(15));

            assert(!a.contains(18));
            a.remove(18);
            assert(!a.contains(18));

            assert(a.contains(0));
            assert(!a.contains(1));
            assert(!a.contains(15));
            assert(a.contains(16));

            // left
            a.add(8);
            assert(a.contains(8));
            a.add(7);
            a.add(9);
            assert(a.contains(8));
            a.remove(7);
            a.remove(9);
            assert(a.contains(8));
            assert(!a.contains(7));
            assert(!a.contains(9));

            // right
            a.add(15);
            assert(a.contains(15));
            a.add(16);
            a.add(14);
            assert(a.contains(15));
            a.remove(14);
            a.remove(16);
            assert(a.contains(15));
            assert(!a.contains(14));
            assert(!a.contains(16));
        }

        void intBitSet_test() {
            suite("IntBitSet");
            test(bit_set);
        }
    }
}



