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
	LinkedListItem* unshift(int value);

	LinkedListItem* insertAfter(int value, LinkedListItem* after);
	LinkedListItem* insertBefore(int value, LinkedListItem* before);


private:
	// disable initialize constructor
	LinkedList(const LinkedList&);

	// disable = operator
	LinkedList& operator= (const LinkedList&);

private:
	int _size;
	LinkedListItem* _first;
	LinkedListItem* _last;

};

//inline LinkedListItem* LinkedList::insert(int value, LinkedListItem* toLinkItem) {
//	LinkedListItem* item = new LinkedListItem(value);
//	_size++;
//	return item;
//}

inline LinkedListItem* LinkedList::push(int value) {
	LinkedListItem* item = new LinkedListItem(value);
	if (!_first) {
		_first = _last = item;
	} else {
		_last->setNext(item);
		_last = item;
	}
	_size++;
	return item;
}

inline LinkedListItem* LinkedList::unshift(int value) {
	LinkedListItem* item = new LinkedListItem(value);
//	if (!_first) {
//		_first = _last = item;
//	} else {
//		_last->setNext(item);
//		_last = item;
//	}
//	_size++;
	return item;
}

}

#endif /* SRC_UTILS_LINKEDLIST_H_ */
