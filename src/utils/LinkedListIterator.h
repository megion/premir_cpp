/*
 * LinkedListIterator.h
 *
 *  Created on: Oct 1, 2015
 *      Author: ilya
 */

#ifndef SRC_UTILS_LINKED_LIST_ITERATOR_H_
#define SRC_UTILS_LINKED_LIST_ITERATOR_H_

#include "LinkedListItem.h"

namespace utils {

template<typename T>
class LinkedListIterator {
public:
	LinkedListIterator(LinkedListItem<T>* current) {
		_current = current;
	}
//	virtual ~LinkedListIterator();

	bool hasNext() {
		if (_current) {
			if (_current->getNext()) {
				return true;
			} else {
				return false;
			}
		}
		return false;
	}

	T next() {
		LinkedListItem<T>* next = _current->getNext();
		_current = next;
		return _current->getValue();
	}

private:
	LinkedListItem<T>* _current;
};

}

#endif
