#include "HeapSortTest.h"

namespace test {
    namespace heap_sort {

        void test_priority_queue() {
            sort::PriorityQueue<int> q;
            size_t childIndex = q.youngChildIndex(1);
            assert(childIndex == 3);

            // parent for left child
            size_t parentIndex = q.parentIndex(childIndex);
            assert(parentIndex == 1);

            // parent for right child
            parentIndex = q.parentIndex(childIndex + 1);
            assert(parentIndex == 1);

            size_t parent3 = 3;
            childIndex = q.youngChildIndex(parent3);
            assert(childIndex == 7);

            parentIndex = q.parentIndex(childIndex);
            assert(parentIndex == parent3);

            assert(q.parentIndex(childIndex+1) == parent3);
            assert(q.parentIndex(childIndex+2) == 4);
        }

        void sort_test() {
            suite("PriorityQueue");
            mytest(priority_queue);

        }
    }
}

