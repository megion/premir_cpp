#include "HeapSortTest.h"

namespace test {
namespace heap_sort {

void test_priority_queue_index_navigation() {
    sort::PriorityQueue<int> q;
    size_t childIndex = q.young_child_index(1);
    assert(childIndex == 3);

    // parent for left child
    size_t parentIndex = q.parent_index(childIndex);
    assert(parentIndex == 1);

    // + testing branch changes

    // parent for right child
    parentIndex = q.parent_index(childIndex + 1);
    assert(parentIndex == 1);

    size_t parent3 = 3;
    childIndex = q.young_child_index(parent3);
    assert(childIndex == 7);

    parentIndex = q.parent_index(childIndex);
    assert(parentIndex == parent3);

    assert(q.parent_index(childIndex + 1) == parent3);
    assert(q.parent_index(childIndex + 2) == 4);

    assert(q.parent_index(5) == 2);
    assert(q.young_child_index(2) == 5);
}

void sort_test() {
    suite("PriorityQueue");
    mytest(priority_queue_index_navigation);
}
} // namespace heap_sort
} // namespace test
