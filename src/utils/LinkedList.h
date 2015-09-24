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

class LinkedList {
public:
	LinkedList() {
		_size = 0;
		_first = nullptr;
		_last = nullptr;
	}

	~LinkedList();

	int size() {
		return _size;
	}

//	LinkedListItem* insert(int value, LinkedListItem* toLinkItem);

	LinkedListItem* push(int value);
	LinkedListItem* unshift(int value); // insert to begin

	LinkedListItem* insertAfter(int value, LinkedListItem* after);
	LinkedListItem* insertBefore(int value, LinkedListItem* before);

	void remove(LinkedListItem* item);
	void removeAll();

private:
	// disable initialize constructor
	LinkedList(const LinkedList&);

	// disable = operator
	LinkedList& operator=(const LinkedList&);

	LinkedListItem* findPrev(LinkedListItem* item) {
		LinkedListItem* p = _first;
		while (p) {
			LinkedListItem* next  = p->getNext();
			if (next && next==item) {
				return p;
			}
			p = next;
		}
		return nullptr;
	}

private:
	int _size;
	LinkedListItem* _first;
	LinkedListItem* _last;

};

inline LinkedListItem* LinkedList::push(int value) {
	LinkedListItem* item = new LinkedListItem(value);
	if (_first) {
		_last->setNext(item);
		_last = item;
	} else {
		_first = _last = item;
	}
	_size++;
	return item;
}

inline LinkedListItem* LinkedList::unshift(int value) {
	LinkedListItem* item = new LinkedListItem(value);
	if (_first) {
		item->setNext(_first);
		_first = item;
	} else {
		_first = _last = item;
	}
	_size++;
	return item;
}

inline LinkedListItem* LinkedList::insertAfter(int value,
		LinkedListItem* after) {
	LinkedListItem* item = new LinkedListItem(value, after->getNext());
	after->setNext(item);
	if (_last == after) {
		_last = item;
	}
	_size++;
	return item;
}

inline LinkedListItem* LinkedList::insertBefore(int value,
		LinkedListItem* before) {
	LinkedListItem* item = new LinkedListItem(value, before);
	if (_first == before) {
		_first = item;
	} else {
		LinkedListItem* prev = findPrev(before);
		// should not be null
		prev->setNext(item);
	}
	_size++;
	return item;
}

}

#endif /* SRC_UTILS_LINKEDLIST_H_ */
