/*
 * LinkedList.h
 *
 *  Created on: Sep 19, 2015
 *      Author: ilya
 */

#ifndef SRC_UTILS_LINKED_LIST_H_
#define SRC_UTILS_LINKED_LIST_H_

#include "LinkedListItem.h"

#include <iostream>

namespace utils {

template<typename T>
class LinkedList {
public:
	LinkedList() {
		_size = 0;
		_first = nullptr;
		_last = nullptr;
	}

	// copy constructor
	LinkedList(const LinkedList<T>& list) :
			_size(0), _first(0), _last(0) {
		concat(list);
	}

	int size() {
		return _size;
	}

	LinkedListItem<T>* push(const T& value); // insert to end
	/**
	 * Remove first item and return new first
	 */
	LinkedListItem<T>* shift(); // delete first

	LinkedListItem<T>* unshift(const T& value); // insert to begin
	/**
	 * Remove last item and return new last
	 */
	LinkedListItem<T>* pop(); // delete last

	LinkedListItem<T>* insertAfter(const T& value, LinkedListItem<T>* after);
	LinkedListItem<T>* insertBefore(const T& value, LinkedListItem<T>* before);

	void concat(const LinkedList<T>& list);
	void reverse(const LinkedList<T>& list);

	void remove(LinkedListItem<T>* item);
	void removeAll();

	class Iterator {
	public:
		Iterator(LinkedListItem<T>* _next) {
			nextItem = _next;
		}

		bool hasNext() {
			return nextItem ? true : false;
		}

		T& next() {
			LinkedListItem<T>* n = nextItem;
			nextItem = nextItem->getNext();
			return n->getValue();
		}

	private:
		LinkedListItem<T>* nextItem;
	};

	// TODO: iterator methods
	Iterator iterator() const {
		return LinkedList<T>::Iterator(_first);
	}

private:
	// disable = operator
	LinkedList& operator=(const LinkedList&);

	LinkedListItem<T>* findPrev(LinkedListItem<T>* item) {
		LinkedListItem<T>* p = _first;
		while (p) {
			LinkedListItem<T>* next = p->getNext();
			if (next && next == item) {
				return p;
			}
			p = next;
		}
		return nullptr;
	}

private:
	int _size;
	LinkedListItem<T>* _first;
	LinkedListItem<T>* _last;

};

}

#endif /* SRC_UTILS_LINKEDLIST_H_ */
