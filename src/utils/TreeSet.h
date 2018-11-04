/**
 * Tree set
 */
#ifndef SRC_UTILS_TREESET_H
#define SRC_UTILS_TREESET_H

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <exception>
#include <iostream>
#include <stdexcept>

namespace utils {

template <typename T>
struct TreeNode {
    T item;
    TreeNode<T>* parent;
    TreeNode<T>* left;
    TreeNode<T>* right;
};

template <typename T>
class TreeSet {
    public:
    TreeSet() {}

    TreeSet<T>(const TreeSet<T>&) = delete; // disable copy constructor
    TreeSet<T>& operator=(const TreeSet<T>&) = delete; // assign operator
    TreeSet<T>& operator=(TreeSet<T>&&) = delete;      // replacement operator

    ~TreeSet() {}

    private:
    TreeNode<T>* node;
};
} // namespace utils

#endif
