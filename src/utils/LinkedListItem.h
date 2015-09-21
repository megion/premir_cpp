/*
 * LinkedListItem.h
 *
 *  Created on: Sep 20, 2015
 *      Author: ilya
 */

#ifndef SRC_UTILS_LINKED_LIST_ITEM_H_
#define SRC_UTILS_LINKED_LIST_ITEM_H_

namespace utils {

class LinkedListItem {
public:
	LinkedListItem(int value) {
		_value = value;
			_next = nullptr;
	}
	LinkedListItem(int value, LinkedListItem* next) {
		_value = value;
		_next = next;
	}

	~LinkedListItem();

	void setNext(LinkedListItem* next) {
		_next = next;
	}

private:
	int _value;
	LinkedListItem* _next;
};

inline LinkedListItem::~LinkedListItem() {
}

} /* namespace utils */

#endif /* SRC_UTILS_LINKEDLISTITEM_H_ */
