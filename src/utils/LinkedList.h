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

template<typename T>
inline LinkedListItem<T>* LinkedList<T>::push(const T& value) {
	LinkedListItem<T>* item = new LinkedListItem<T>(value);
	if (_first) {
		_last->setNext(item);
		_last = item;
	} else {
		_first = _last = item;
	}
	_size++;
	return item;
}

template<typename T>
inline LinkedListItem<T>* LinkedList<T>::unshift(const T& value) {
	LinkedListItem<T>* item = new LinkedListItem<T>(value);
	if (_first) {
		item->setNext(_first);
		_first = item;
	} else {
		_first = _last = item;
	}
	_size++;
	return item;
}

template<typename T>
inline LinkedListItem<T>* LinkedList<T>::shift() {
	// delete first item
	if (_first) {
		LinkedListItem<T>* next = _first->getNext();
		delete _first;
		_size--;
		if (next) {
			_first = next;
		} else {
			_first = _last = nullptr;
		}
		return _first;
	} else {
		// no items
		return nullptr;
	}
}

template<typename T>
inline LinkedListItem<T>* LinkedList<T>::pop() {
	// delete last item
	if (_last) {
		LinkedListItem<T>* prev = findPrev(_last);
		delete _last;
		_size--;
		if (prev) {
			prev->setNext(nullptr);
			_last = prev;
		} else {
			_last = _first = nullptr;
		}

		return _last;
	} else {
		return nullptr;
	}
}

template<typename T>
inline LinkedListItem<T>* LinkedList<T>::insertAfter(const T& value,
		LinkedListItem<T>* after) {
	LinkedListItem<T>* item = new LinkedListItem<T>(value, after->getNext());
	after->setNext(item);
	if (_last == after) {
		_last = item;
	}
	_size++;
	return item;
}

template<typename T>
inline LinkedListItem<T>* LinkedList<T>::insertBefore(const T& value,
		LinkedListItem<T>* before) {
	LinkedListItem<T>* item = new LinkedListItem<T>(value, before);
	if (_first == before) {
		_first = item;
	} else {
		LinkedListItem<T>* prev = findPrev(before);
		// should not be null
		prev->setNext(item);
	}
	_size++;
	return item;
}

template<typename T>
inline void LinkedList<T>::concat(const LinkedList<T>& list) {
	LinkedListItem<T>* pi = list._first;
	while (pi) {
		push(pi->getValue());
		pi = pi->getNext();
	}
}

}

#endif /* SRC_UTILS_LINKEDLIST_H_ */
