#ifndef SRC_UTILS_C_ARRAY_LIST_H_
#define SRC_UTILS_C_ARRAY_LIST_H_

#include <cstdio>
#include <cstdlib>
#include <cerrno>
//#include <string.h>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

namespace utils {

/**
 * Реализация списка в виде массива, которая предназначена для использования
 * только с простыми типами: int, double, и др. а также с простыми структурами
 * или даже классами. Простыми в данном случае называем такие типы которые
 * внутри семя не выделяют динамическую память и не освобождают ее
 * (new и delete).
 * Такое требование к типам хранимых данных определяется тем, что реализация
 * использует для выделения динамической памяти C функции malloc и realloc,
 * а для ее освобождения free.
 * Класс сконструирован исключительно в интересах производительности.
 */
    template<typename T>
    class CArrayList {
    public:

        CArrayList() :
                length(0), capacity(0), capacityIncrease(1), //
                typeSizeof(sizeof(T)), array(nullptr) {
        }

        CArrayList(size_t _capacity, size_t _capacityIncrease = 1) :
                length(0), capacity(_capacity), //
                capacityIncrease(_capacityIncrease), typeSizeof(sizeof(T)),
				array(nullptr) {
            if (capacity == 0) {
                array = nullptr;
            } else {
                size_t amount = capacity * typeSizeof;
                array = (T *) std::malloc(amount);
                if (array == NULL) {
                    throw std::runtime_error(std::strerror(errno));
                }
            }
        }

        // copy constructor: List l1; List l2 = l1;
        CArrayList(const CArrayList<T> &list) :
                length(list.length), capacity(list.capacity), //
                capacityIncrease(list.capacityIncrease), typeSizeof(sizeof(T)) {
            if (capacity == 0) {
                array = nullptr;
            } else {
                size_t amount = capacity * typeSizeof;
                array = (T *) std::malloc(amount);
                if (array == NULL) {
                    throw std::runtime_error(std::strerror(errno));
                }

                // copy all array
                std::memcpy(array, list.array, amount);
            }
        }

        // replace constructor: List l1; List l2(std::move(l1));
        CArrayList(CArrayList<T> &&list) :
                length(list.length), capacity(list.capacity), //
                capacityIncrease(list.capacityIncrease),
                typeSizeof(sizeof(T)), //
                array(list.array) {
            list.length = 0;
            list.capacity = 0;
            list.array = nullptr;
        }

        ~CArrayList() {
            removeAll();
        }

        size_t size() const {
            return length;
        }

		/**
		 * sometimes need set size.
		 * For example when array created with specified capacity 
		 * and fill array without pushing elements
		 */
		void setSize(size_t size) {
            length = size;
        }

        T *getArray() const {
            return array;
        }

        size_t getTypeSizeof() const {
            return typeSizeof;
        }

        // = assign operator: List l3; l3 = l2;
        CArrayList<T> &operator=(const CArrayList<T> &) = delete;

        // = replacement operator: List l3; l3 = std::move(l2);
        CArrayList<T> &operator=(CArrayList<T>&& list) {
			length = list.length;
			capacity = list.capacity;
			capacityIncrease = list.capacityIncrease;
			array = list.array;
			typeSizeof = sizeof(T);

            list.length = 0;
            list.capacity = 0;
            list.array = nullptr;
		}	

        // [] index operator
        T &operator[](const size_t &index) const {
            return array[index];
        }

        bool operator==(const CArrayList<T> &other) const {
            if (other.size() != length) {
                return false;
            }
            for (size_t i = 0; i < length; ++i) {
                if(array[i] != other[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const CArrayList<T> &other) const {
            return !((*this) == other);
        }

        // + sum operator: a = a + b
        CArrayList<T> sum(const CArrayList<T> &b) {
            for (size_t i = 0; i < length; ++i) {
                array[i] = array[i] + b.array[i];
            }
            return *this;
        }

        // iterator implementation for use in C++11 range-based for loops
        class Iterator {
        public:
            Iterator(T *_arr, size_t _pos) :
                    arr(_arr), pos(_pos) {
            }

            bool operator!=(const Iterator &other) const {
                return pos != other.pos;
            }

            T &operator*() const {
                return arr[pos];
            }

            const Iterator &operator++() {
                ++pos;
                return *this;
            }

        private:
            size_t pos;
            T *arr;
        };

        // begin method range-based for loop
        Iterator begin() const {
            return Iterator(array, 0);
        }

        // begin method range-based for loop
        Iterator end() const {
            return Iterator(array, length);
        }
        /////////////////////////////////

        /**
         * Insert to end
         */
        void push(const T &value) {
            push(&value, 1);
        }

        void push(const T *values, size_t valuesSize) {
            size_t newLength = length + valuesSize;
			size_t oldLength = length;
			initializeArrayMemory(newLength);

            // copy value as bytes. Copy constructor not call.
            memcpy(array + oldLength, values, valuesSize * typeSizeof);
        }

        /**
         * Save copy values begin with specified position
         */
        void write(size_t position, const T *values,  size_t valuesSize) {
            size_t newLength = position + valuesSize;
            if (newLength > capacity) { // need increase capacity
				initializeArrayMemory(newLength);
            } else if (newLength > length) { // simple update length
                length = newLength;
            }

            // copy value as bytes. Copy constructor not call.
            memcpy(array + position, values, valuesSize * typeSizeof);
        }

        void write(size_t position, const T &value) {
            write(position, &value, 1);
        }

        /**
         * Remove all elements
         */
        void removeAll() {
            if (array) {
                std::free(array);
            }
            array = nullptr;
            length = 0;
            capacity = 0;
        }

        void print(const char* separator=", ") {
            for(size_t i=0; i<length; ++i) {
                std::cout << array[i] << separator;
            }
            std::cout << std::endl;
        }


    private:
        T *array;
        size_t length; // current list size (is number contained elements)
        size_t capacity; // list capacity or max size
        size_t typeSizeof; // saved value sizeof
        size_t capacityIncrease; // increase capacity value, default is 1

		void initializeArrayMemory(size_t newSize) {
			size_t newCapacity = 0;
			if (newSize > (capacity + capacityIncrease)) {
				newCapacity = newSize;
			} else {
				newCapacity = capacity + capacityIncrease;
			}

			size_t amount = typeSizeof * newCapacity;
			T *newArray;
			if (array) {
				newArray = (T *) std::realloc(array, amount);
			} else {
				newArray = (T *) std::malloc(amount);
			}

			if (newArray == NULL) {
				throw std::runtime_error(std::strerror(errno));
			}
			array = newArray;
			length = newSize;
			capacity = newCapacity;
		}
    };

}

#endif
