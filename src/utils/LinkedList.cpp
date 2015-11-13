/*
 * LinkedList.cpp
 *
 *  Created on: Sep 19, 2015
 *      Author: ilya
 */

#include "LinkedList.h"

namespace utils {

template<typename T>
LinkedListItem<T>* LinkedList<T>::push(const T& value) {
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
LinkedListItem<T>* LinkedList<T>::unshift(const T& value) {
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
LinkedListItem<T>* LinkedList<T>::shift() {
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
LinkedListItem<T>* LinkedList<T>::pop() {
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
LinkedListItem<T>* LinkedList<T>::insertAfter(const T& value,
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
LinkedListItem<T>* LinkedList<T>::insertBefore(const T& value,
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
void LinkedList<T>::concat(const LinkedList<T>& list) {
	LinkedListItem<T>* pi = list._first;
	while (pi) {
		push(pi->getValue());
		pi = pi->getNext();
	}
}

}


