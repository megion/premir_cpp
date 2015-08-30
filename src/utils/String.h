#ifndef UTILS_STRING_H
#define UTILS_STRING_H

#include <iostream>
#include <cstring>
#include <iomanip>

namespace utils {

class String {

public:
	String();
	String(const char*);
	String(const String&);

	~String();

	String& operator =(const String&); // str1 = str2
	String& operator =(const char*); // str3 = "my string";

	bool operator ==(const String&);
	bool operator ==(const char*);

	char& operator [](int);

	int size() {
		return _size;
	}

	const char* getStr() {
		return _str;
	}

private:
	void initializeNull() {
		_size = 0;
		_str = nullptr;
	}

private:
	int _size;
	/**
	 * UTF8 это многобайтная кодировка символов элементами в 1 байт,
	 * а UTF16 элементами размером в 2 байта.
	 * UTF-8 1-4 байт на символ.
	 * UTF-16 2-4 байт на символ.
	 */
	char* _str;
};

// constructors
inline String::String() {
	initializeNull();
}
inline String::String(const char* str) {
	if (!str) {
		initializeNull();
		return;
	}

	_size = std::strlen(str);
	_str = new char[_size + 1];
	std::strcpy(_str, str);
}
inline String::String(const String& rhs) {
	_size = rhs._size;
	if (rhs._str) {
		_str = nullptr;
	} else {
		_str = new char[_size + 1];
		std::strcpy(_str, rhs._str);
	}

}

// destructor
inline String::~String() {
	delete[] _str;
}

// comparators
inline bool String::operator==(const String& rhs) {
	if (_size != rhs._size) {
		return false;
	}
	return std::strcmp(_str, rhs._str) ? false : true;
}
inline bool String::operator==(const char* s) {
	return std::strcmp(_str, s) ? false : true;
}

// = operator
inline String& String::operator=(const char* s) {
	delete[] _str;
	if (s) {
		_size = std::strlen(s);
		_str = new char[_size + 1];
		std::strcpy(_str, s);
	} else {
		initializeNull();
	}
	return *this;
}
inline String& String::operator=(const String& rhs) {
	if (this != &rhs) { // check itself object
		delete[] _str;
		if (rhs._str) {
			_size = rhs._size;
			_str = new char[_size + 1];
			std::strcpy(_str, rhs._str);
		} else {
			initializeNull();
		}
	}
	return *this;
}

// [] operator
inline char& String::operator[](int ind) {
	return _str[ind];
}

// >> operator
inline std::istream& operator>>(std::istream& is, String& s) {
	const int limit = 4096;
	char inBuf[limit];
	is >> std::setw(limit) >> inBuf;
	s = inBuf;
	return is;
}

// << operator
inline std::ostream& operator<<(std::ostream& os, String& s) {
	return os << s.getStr();
}

}

#endif
