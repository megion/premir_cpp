/**
 */
#ifndef SRC_SORT_PRIORITY_QUEUE_H
#define SRC_SORT_PRIORITY_QUEUE_H

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace sort {

// template <typename T>
// struct TreeNode {
// T item;
// TreeNode<T>* parent;
// TreeNode<T>* left;
// TreeNode<T>* right;
//};

template <typename T>
class PriorityQueue {
    public:
    PriorityQueue() : length(0) {
        queue = new std::vector<T>();
    }

    PriorityQueue<T>(const PriorityQueue<T>&) = delete;

    PriorityQueue<T>& operator=(const PriorityQueue<T>&) = delete;

    PriorityQueue<T>& operator=(PriorityQueue<T>&&) = delete;

    ~PriorityQueue() {
        delete queue;
        queue = nullptr;
        length = 0;
    }

    /**
     * left child = parent*2 + 1
     */
    inline size_t youngChildIndex(size_t index) const {
        return index*2 + 1;
    }

    /**
     * parent = (child - 1) / 2
     */
    inline size_t parentIndex(size_t index) const {
        return ((size_t)(index-1)/2);
    }

    private:
    std::vector<T>* queue;
    size_t length; // number contained elements
};
} // namespace sort

#endif
