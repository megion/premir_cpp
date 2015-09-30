/*
 * LinkedList.h
 *
 *  Created on: Sep 19, 2015
 *      Author: ilya
 */

#ifndef SRC_UTILS_LINKED_LIST_H_
#define SRC_UTILS_LINKED_LIST_H_

#include "LinkedListItem.h"

namespace utils {

template <typename T>
class LinkedList {
public:
	LinkedList() {
		_size = 0;
		_first = nullptr;
		_last = nullptr;
	}

	virtual ~LinkedList();

	int size() {
		return _size;
	}

	LinkedListItem<T>* push(T value); // insert to end
	LinkedListItem<T>* shift(); // delete from begin

	LinkedListItem<T>* unshift(T value); // insert to begin
	LinkedListItem<T>* pop(); // delete from end

	LinkedListItem<T>* insertAfter(T value, LinkedListItem<T>* after);
	LinkedListItem<T>* insertBefore(T value, LinkedListItem<T>* before);

	void concat(const LinkedList& list);
	void reverse(const LinkedList& list);

	void remove(LinkedListItem<T>* item);
	void removeAll();

private:
	// disable initialize constructor
	LinkedList(const LinkedList&);

	// disable = operator
	LinkedList& operator=(const LinkedList&);

	LinkedListItem<T>* findPrev(LinkedListItem<T>* item) {
		LinkedListItem<T>* p = _first;
		while (p) {
			LinkedListItem<T>* next  = p->getNext();
			if (next && next==item) {
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

template <typename T>
inline LinkedListItem<T>* LinkedList<T>::push(T value) {
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

template <typename T>
inline LinkedListItem<T>* LinkedList<T>::unshift(T value) {
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

template <typename T>
inline LinkedListItem<T>* LinkedList<T>::shift() {
	// delete first item
	return nullptr;
}

template <typename T>
inline LinkedListItem<T>* LinkedList<T>::pop() {
	// delete end element
	return nullptr;
}

template <typename T>
inline LinkedListItem<T>* LinkedList<T>::insertAfter(T value,
		LinkedListItem<T>* after) {
	LinkedListItem<T>* item = new LinkedListItem<T>(value, after->getNext());
	after->setNext(item);
	if (_last == after) {
		_last = item;
	}
	_size++;
	return item;
}

template <typename T>
inline LinkedListItem<T>* LinkedList<T>::insertBefore(T value,
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

}

#endif /* SRC_UTILS_LINKEDLIST_H_ */
