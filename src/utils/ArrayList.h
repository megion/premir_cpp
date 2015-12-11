/*
 * LinkedList.h
 *
 *  Created on: Sep 19, 2015
 *      Author: ilya
 */

#ifndef SRC_UTILS_LINKED_LIST_H_
#define SRC_UTILS_LINKED_LIST_H_

// std::move
#include <utility>
#include <iostream>

namespace utils {

/**
 * Simple and very fast linked list realization of list.
 */
template<typename T>
class ArrayList {
public:

	ArrayList() :
			size(0) {
	}

	// copy constructor: LinkedList l1; LinkedList l2 = l1;
	ArrayList(const ArrayList<T>& list) :
			size(0){
		concat(list);
	}

	// replace constructor: LinkedList l1; LinkedList l2(std::move(l1));
	ArrayList(ArrayList<T> && list) {
	}

	~ArrayList();

	size_t size() const {
		return size;
	}

	/**
	 * Insert to end
	 */
	void push(const T& value);
	void push(T&& value);

	/**
	 * Remove first item and return new first
	 */
	void shift();

	/**
	 * Insert to begin.
	 */
	void unshift(const T& value);
	void unshift(T&& value);

	/**
	 * Remove last item and return new last
	 */
	void pop();

	void concat(const LinkedList<T>& list);

	void reverse();

	/**
	 * Remove all elements
	 */
	void removeAll();

	/**
	 * Remove all elements from list and destroy each.
	 * Method must be call only if each element was created as dynamic (new ...)
	 */
	void removeAndDestroyAll();

	// = assign operator: LinkedList l3; l3 = l2;
	ArrayList<T>& operator=(const LinkedList<T>&);
	// = replacement operator: LinkedList l3; l3 = std::move(l2);
	LinkedList<T>& operator=(LinkedList<T> &&) = delete; //

	// [] index operator
	T& operator[](const size_t& index);

private:


private:
	size_t size;

};

}

#endif /* SRC_UTILS_LINKEDLIST_H_ */
