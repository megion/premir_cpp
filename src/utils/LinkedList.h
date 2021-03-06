/**
 * Simple linked list.
 */
#ifndef SRC_UTILS_LINKED_LIST_H_
#define SRC_UTILS_LINKED_LIST_H_

// std::move
#include <utility>
#include <iostream>

namespace utils {

    template<typename T>
    class LinkedList {
    public:

        LinkedList() :
                _size(0), _first(nullptr), _last(nullptr) {
        }

        // copy constructor: LinkedList l1; LinkedList l2 = l1;
        LinkedList(const LinkedList<T> &list) :
                _size(0), _first(nullptr), _last(nullptr) {
            concat(list);
        }

        // replace constructor: LinkedList l1; LinkedList l2(std::move(l1));
        LinkedList(LinkedList<T> &&list) :
                _size(list._size), _first(list._first), _last(list._last) {
            list._size = 0;
            list._first = nullptr;
            list._last = nullptr;
        }

        ~LinkedList();

        size_t size() const {
            return _size;
        }

        //////////////////////////////// Entry
        class Entry {
        public:
            Entry(const T &value) :
                    _value(value), _next(nullptr) {
                //  TODO: _value(value) run copy constructor here. It need for store
                // local values, for example in cycle:
                // for (int i = 0; i < 100; ++i) {
                //     Foo v = Foo('1', '2', i, i * 2);
                //     list.push(v);
                // }
//			std::cout << "copy entry: " << &_value << std::endl;
            }

            Entry(T &&value) :
                    _value(std::move(value)), _next(nullptr) {
//			std::cout << "move entry: " << &_value << std::endl;
            }

            Entry(const T &value, Entry *next) :
                    _value(value), _next(next) {
            }

            void setNext(Entry *next) {
                _next = next;
            }

            Entry *getNext() const {
                return _next;
            }

            T &getValue() {
                return _value;
            }

        private:
            T _value; // TODO: variable is copy
            Entry *_next;
        };
        /////////////////////////////////

        ///////////////////////////////// Iterator
        class Iterator {
        public:
            Iterator(Entry *_next) {
                nextItem = _next;
            }

            bool hasNext() const {
                return nextItem ? true : false;
            }

            Entry *next() {
                Entry *n = nextItem;
                nextItem = nextItem->getNext();
                return n;
            }

        private:
            Entry *nextItem;
        };
        /////////////////////////////////

        /**
         * Insert to end
         */
        Entry *push(const T &value);

        Entry *push(T &&value);

        /**
         * Remove first item and return new first
         */
        Entry *shift();

        /**
         * Insert to begin.
         */
        Entry *unshift(const T &value);

        Entry *unshift(T &&value);

        /**
         * Remove last item and return new last
         */
        Entry *pop();

        Entry *insertAfter(const T &value, Entry *after);

        Entry *insertAfter(T &&value, Entry *after);

        Entry *insertBefore(const T &value, Entry *before);

        Entry *insertBefore(T &&value, Entry *before);

        void concat(const LinkedList<T> &list);

        void reverse();

        /**
         * Replace a->b and b->a
         */
        void swap(Entry *a, Entry *b);

        /**
         * Remove specified entry
         */
        void remove(Entry *item);

        /**
         * Remove all elements
         */
        void removeAll();

        /**
         * Remove all elements from list and destroy each.
         * Method must be call only if each element was created as dynamic (new ...)
         */
        void removeAndDestroyAll();

        Iterator iterator() const {
            return Iterator(_first);
        }

        // = assign operator: LinkedList l3; l3 = l2;
        LinkedList<T> &operator=(const LinkedList<T> &);

        // = replacement operator: LinkedList l3; l3 = std::move(l2);
        LinkedList<T> &operator=(LinkedList<T> &&) = delete; //

        // [] index operator
        Entry *operator[](const size_t &index);

    private:
        /**
         * Find previous entry (next == item)
         */
        Entry *findPrevious(Entry *item) const {
            Entry *p = _first;
            while (p) {
                Entry *next = p->getNext();
                if (next && next == item) {
                    return p;
                }
                p = next;
            }
            return nullptr;
        }

        void pushEntry(Entry *item) {
            if (_first) {
                _last->setNext(item);
                _last = item;
            } else {
                _first = _last = item;
            }
            _size++;
        }

        void unshiftEntry(Entry *item) {
            if (_first) {
                item->setNext(_first);
                _first = item;
            } else {
                _first = _last = item;
            }
            _size++;
        }

        void insertEntryAfter(Entry *item, Entry *after) {
            after->setNext(item);
            if (_last == after) {
                _last = item;
            }
            _size++;
        }

        void insertEntryBefore(Entry *item, Entry *before) {
            if (_first == before) {
                _first = item;
            } else {
                Entry *prev = findPrevious(before);
                // should not be null
                prev->setNext(item);
            }
            _size++;
        }

    private:
        size_t _size;
        Entry *_first;
        Entry *_last;

    };

    template<typename T>
    LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &list) {
        if (this == &list) {
            //protect self assignment
            return *this;
        }

        removeAll();
        concat(list);
        return *this;
    }

    template<typename T>
    typename LinkedList<T>::Entry *LinkedList<T>::operator[](const size_t &index) {
        Entry *pi = _first;
        size_t i = 0;
        while (pi) {
            if (i == index) {
                return pi;
            }
            pi = pi->getNext();
            ++i;
        }

        return nullptr;
    }

    template<typename T>
    void LinkedList<T>::removeAll() {
        // Release only links (linked items),
        // but no released elements
        Entry *pi = _first;
        while (pi) {
            Entry *prev = pi;
            pi = pi->getNext();
            delete prev;
        }
        _first = nullptr;
        _last = nullptr;
        _size = 0;
    }

    template<typename T>
    void LinkedList<T>::removeAndDestroyAll() {
        // Release only links (linked items),
        // but no released elements
        Entry *pi = _first;
        while (pi) {
            Entry *prev = pi;
            pi = pi->getNext();
            delete prev->getValue(); // destroy element
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
    typename LinkedList<T>::Entry *LinkedList<T>::push(const T &value) {
        Entry *item = new Entry(value);
        pushEntry(item);
        return item;
    }

    template<typename T>
    typename LinkedList<T>::Entry *LinkedList<T>::push(T &&value) {
        Entry *item = new Entry(std::move(value));
        pushEntry(item);
        return item;
    }

    template<typename T>
    typename LinkedList<T>::Entry *LinkedList<T>::shift() {
        // delete first item
        if (_first) {
            Entry *next = _first->getNext();
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
    typename LinkedList<T>::Entry *LinkedList<T>::unshift(const T &value) {
        Entry *item = new Entry(value);
        unshiftEntry(item);
        return item;
    }

    template<typename T>
    typename LinkedList<T>::Entry *LinkedList<T>::unshift(T &&value) {
        Entry *item = new Entry(std::move(value));
        unshiftEntry(item);
        return item;
    }

    template<typename T>
    typename LinkedList<T>::Entry *LinkedList<T>::pop() {
        // delete last item
        if (_last) {
            Entry *prev = findPrevious(_last);
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
    typename LinkedList<T>::Entry *LinkedList<T>::insertAfter(const T &value,
                                                              Entry *after) {
        Entry *item = new Entry(value, after->getNext());
        insertEntryAfter(item, after);
        return item;
    }

    template<typename T>
    typename LinkedList<T>::Entry *LinkedList<T>::insertAfter(T &&value,
                                                              Entry *after) {
        Entry *item = new Entry(std::move(value), after->getNext());
        insertEntryAfter(item, after);
        return item;
    }

    template<typename T>
    typename LinkedList<T>::Entry *LinkedList<T>::insertBefore(const T &value,
                                                               Entry *before) {
        Entry *item = new Entry(value, before);
        insertEntryBefore(item, before);
        return item;
    }

    template<typename T>
    typename LinkedList<T>::Entry *LinkedList<T>::insertBefore(T &&value,
                                                               Entry *before) {
        Entry *item = new Entry(std::move(value), before);
        insertEntryBefore(item, before);
        return item;
    }

    template<typename T>
    void LinkedList<T>::concat(const LinkedList<T> &list) {
        Entry *pi = list._first;
        while (pi) {
            push(pi->getValue());
            pi = pi->getNext();
        }
    }

}

#endif
