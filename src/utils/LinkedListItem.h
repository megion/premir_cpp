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
	LinkedListItem(int value, LinkedListItem* toLinkItem);

	~LinkedListItem();

private:
	int _value;
	LinkedListItem* _next;
};

/**
 * Create item inserted after toLinkItem
 */
inline LinkedListItem::LinkedListItem(int value, LinkedListItem* toLinkItem) {
	_value = value;

	// this links with toLinkItem->_next
	_next = toLinkItem->_next;
	// toLinkItem links with this
	toLinkItem->_next = this;
}

} /* namespace utils */

#endif /* SRC_UTILS_LINKEDLISTITEM_H_ */
