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
	LinkedListIterator(LinkedListItem<T>* next) {
		nextItem = next;
	}

	bool hasNext() {
		return nextItem?true:false;
	}

	LinkedListItem<T>* next() {
		LinkedListItem<T>* n = nextItem;
		nextItem = nextItem->getNext();
		return n;
	}

private:
	LinkedListItem<T>* nextItem;
};

}

#endif
