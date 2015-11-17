/*
 * LinkedList.h
 *
 *  Created on: Sep 19, 2015
 *      Author: ilya
 */

#ifndef SRC_UTILS_LINKED_LIST_H_
#define SRC_UTILS_LINKED_LIST_H_

#include <iostream>

namespace utils {

/**
 * Simple and very fast linked list realization of list.
 */
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

	~LinkedList();

	int size() const {
		return _size;
	}

	//////////////////////////////// Entry
	class Entry {
	public:
		Entry(const T& value) {
			_value = value;
			_next = nullptr;
		}
		Entry(const T& value, Entry* next) {
			_value = value;
			_next = next;
		}

		void setNext(Entry* next) {
			_next = next;
		}

		Entry* getNext() {
			return _next;
		}

		T& getValue() {
			return _value;
		}

	private:
		T _value;
		Entry* _next;
	};
	/////////////////////////////////

	///////////////////////////////// Iterator
	class Iterator {
	public:
		Iterator(Entry* _next) {
			nextItem = _next;
		}

		bool hasNext() {
			return nextItem ? true : false;
		}

		Entry* next() {
			Entry* n = nextItem;
			nextItem = nextItem->getNext();
			return n;
		}

	private:
		Entry* nextItem;
	};
	/////////////////////////////////

	/**
	 * Insert to end
	 */
	Entry* push(const T& value);

	/**
	 * Remove first item and return new first
	 */
	Entry* shift();

	/**
	 * Insert to begin.
	 */
	Entry* unshift(const T& value);

	/**
	 * Remove last item and return new last
	 */
	Entry* pop();

	Entry* insertAfter(const T& value, Entry* after);

	Entry* insertBefore(const T& value, Entry* before);

	void concat(const LinkedList<T>& list);

	void reverse();
	/**
	 * Replace a->b and b->a
	 */
	void swap(Entry* a, Entry* b);

	void remove(Entry* item);
	void removeAll();

	Iterator iterator() const {
		return LinkedList<T>::Iterator(_first);
	}

	// = operator
	LinkedList<T>& operator=(const LinkedList<T>&);

private:
	/**
	 * Find previous entry (next == item)
	 */
	Entry* findPrevious(Entry* item) {
		Entry* p = _first;
		while (p) {
			Entry* next = p->getNext();
			if (next && next == item) {
				return p;
			}
			p = next;
		}
		return nullptr;
	}

private:
	int _size;
	Entry* _first;
	Entry* _last;

};

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& list) {
	removeAll();
	concat(list);
	return *this;
}

template<typename T>
void LinkedList<T>::removeAll() {
	// Release only links (linked items),
	// but no released elements
	Entry* pi = _first;
	while (pi) {
		Entry* prev = pi;
		pi = pi->getNext();
		delete prev;
	}
	_first = nullptr;
	_last = nullptr;
	_size = 0;
}

template<typename T>
LinkedList<T>::~LinkedList() {
	removeAll();
}

template<typename T>
typename LinkedList<T>::Entry* LinkedList<T>::push(const T& value) {
	Entry* item = new Entry(value);
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
typename LinkedList<T>::Entry* LinkedList<T>::shift() {
	// delete first item
	if (_first) {
		Entry* next = _first->getNext();
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
typename LinkedList<T>::Entry* LinkedList<T>::unshift(const T& value) {
	Entry* item = new Entry(value);
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
typename LinkedList<T>::Entry* LinkedList<T>::pop() {
	// delete last item
	if (_last) {
		Entry* prev = findPrevious(_last);
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
typename LinkedList<T>::Entry* LinkedList<T>::insertAfter(const T& value,
		Entry* after) {
	Entry* item = new Entry(value, after->getNext());
	after->setNext(item);
	if (_last == after) {
		_last = item;
	}
	_size++;
	return item;
}

template<typename T>
typename LinkedList<T>::Entry* LinkedList<T>::insertBefore(const T& value,
		Entry* before) {
	Entry* item = new Entry(value, before);
	if (_first == before) {
		_first = item;
	} else {
		Entry* prev = findPrevious(before);
		// should not be null
		prev->setNext(item);
	}
	_size++;
	return item;
}

template<typename T>
void LinkedList<T>::concat(const LinkedList<T>& list) {
	Entry* pi = list._first;
	while (pi) {
		push(pi->getValue());
		pi = pi->getNext();
	}
}

}

#endif /* SRC_UTILS_LINKEDLIST_H_ */
