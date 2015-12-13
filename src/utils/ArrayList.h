#ifndef SRC_UTILS_ARRAY_LIST_H_
#define SRC_UTILS_ARRAY_LIST_H_

#include <utility>
#include <iostream>

namespace utils {

/**
 * Simple array list.
 */
template<typename T>
class ArrayList {
public:

	ArrayList() :
			length(0), capacity(0), array(nullptr) {
	}

	ArrayList(size_t _capacity) :
			length(0), capacity(_capacity) {
		if (capacity == 0) {
			array = nullptr;
		} else {
			array = new T[capacity];
		}
	}

	// copy constructor: List l1; List l2 = l1;
	ArrayList(const ArrayList<T>& list) :
			length(0), capacity(list.length) {
		if (capacity == 0) {
			array = nullptr;
		} else {
			array = new T[capacity];
			concat(list);
		}
	}

	// replace constructor: List l1; List l2(std::move(l1));
	ArrayList(ArrayList<T> && list) :
		length(list.length), capacity(list.capacity), array(list.array){
		list.length = 0;
		list.capacity = 0;
		list.array = nullptr;
	}

	~ArrayList() {
		if (array) {
			delete[] array;
		}
		array = nullptr;
		length = 0;
		capacity = 0;
	}

	size_t size() const {
		return length;
	}

	//////////////////////////////// Entry
//	class Entry {
//	public:
//		Entry(const T& _value) :
//				value(_value) {
//			// TODO: value(_value) run copy constructor here.
//			 std::cout << "copy entry: " << &_value << std::endl;
//		}
//		Entry(T&& _value) :
//				value(std::move(_value)) {
//			 std::cout << "move entry: " << &_value << std::endl;
//		}
//
//		// copy operator
//		Entry& operator=(const Entry& other) {
////			value(other.value);
//			return *this;
//		}
//		// replacement operator
//		Entry& operator=(Entry&&) = delete;
//
//		T& getValue() {
//			return value;
//		}
//
//	private:
//		T value; // TODO: variable is copy
//	};
	/////////////////////////////////

	/**
	 * Insert to end
	 */
	void push(const T& value);
	void push(T&& value);

	/**
	 * Remove last
	 */
	void pop();

	void concat(const LinkedList<T>& list);

	void reverse();

	/**
	 * Remove all elements
	 */
	void removeAll();

	/**
	 * Remove all elements from list and destroy each.
	 * Method must be call only if each element was created as dynamic (new ...)
	 */
	void removeAndDestroyAll();

	// = assign operator: List l3; l3 = l2;
	ArrayList<T>& operator=(const ArrayList<T>&);
	// = replacement operator: List l3; l3 = std::move(l2);
	ArrayList<T>& operator=(ArrayList<T> &&) = delete; //

	// [] index operator
	T& operator[](const size_t& index);

private:
	T* array;
	size_t length; // current list size (is number contained elements)
	size_t capacity; // list capacity or max size

};

template<typename T>
void ArrayList<T>::push(const T& value) {
	if (capacity > length) {

	} else {
		// increase array and capacity by one
	}

	return;
}

}

#endif
