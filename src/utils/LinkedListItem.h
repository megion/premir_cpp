/*
 * LinkedListItem.h
 *
 *  Created on: Sep 20, 2015
 *      Author: ilya
 */

#ifndef SRC_UTILS_LINKED_LIST_ITEM_H_
#define SRC_UTILS_LINKED_LIST_ITEM_H_

namespace utils {

template<typename T>
class LinkedListItem {
public:
	LinkedListItem(T value) {
		_value = value;
		_next = nullptr;
	}
	LinkedListItem(T value, LinkedListItem* next) {
		_value = value;
		_next = next;
	}

//	virtual ~LinkedListItem();

	void setNext(LinkedListItem* next) {
		_next = next;
	}

	LinkedListItem* getNext() {
		return _next;
	}

	T getValue() {
		return _value;
	}

private:
	T _value;
	LinkedListItem* _next;
};

//inline LinkedListItem::~LinkedListItem() {
//}

} /* namespace utils */

#endif /* SRC_UTILS_LINKEDLISTITEM_H_ */
