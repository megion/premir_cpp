/**
 * Skiena book 
 * 4.3. Пирамидальная сортировка
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
    PriorityQueue() {
        queue = new std::vector<T>();
    }

    PriorityQueue<T>(const PriorityQueue<T>&) = delete;

    PriorityQueue<T>& operator=(const PriorityQueue<T>&) = delete;

    PriorityQueue<T>& operator=(PriorityQueue<T>&&) = delete;

    ~PriorityQueue() {
        delete queue;
        queue = nullptr;
    }

    /**
     * left child = parent*2 + 1
     */
    inline size_t young_child_index(size_t index) const {
        return index*2 + 1;
    }

    /**
     * parent = (child - 1) / 2
     */
    inline size_t parent_index(size_t index) const {
        return ((size_t)(index-1)/2);
    }

    /**
     * insert item
     */
    void insert(const T &item) {
        queue->push_back(item);
        bubble_up(queue->size() - 1);
    }

    /**
     * bubble up for item in position index
     */
    void bubble_up(size_t index) {
        if(index == 0) {
            return; /* root element */
        }

        size_t parentIndex = parent_index(index);
        T& parent = *queue[parentIndex];
        T& item = *queue[index];
        if(parent > item) {
            std::swap(parent, item);
            //queue->assign(parentIndex, item);
        }


    }

    private:
    std::vector<T>* queue;
};
} // namespace sort

#endif
