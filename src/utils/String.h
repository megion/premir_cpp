#ifndef SRC_UTILS_STRING_H_
#define SRC_UTILS_STRING_H_

#include <iostream>
#include <cstring>
#include <iomanip>

namespace utils {

class String {

public:
	String() :
			length(0), str(nullptr) {
	}

	String(const char* _str) :
			length(std::strlen(_str)), str(nullptr) {
		str = new char[length + 1];
		std::strncpy(str, _str, length);
	}

	String(const String& rhs) : length(rhs.length), str(nullptr) {
		if (rhs.str) {
			str = new char[length + 1];
			std::strncpy(str, rhs.str, length);
		}
	}

	~String() {
		length = 0;
		delete[] str;
	}

	String& operator =(const String&); // str1 = str2
	String& operator =(const char*); // str3 = "my string";

	bool operator ==(const String&);
	bool operator ==(const char*);

	char& operator [](int);

	size_t size() {
		return length;
	}

	const char* getChars() {
		return str;
	}

private:
	size_t length;
	/**
	 * UTF8 это многобайтная кодировка символов элементами в 1 байт,
	 * а UTF16 элементами размером в 2 байта.
	 * UTF-8 1-4 байт на символ.
	 * UTF-16 2-4 байт на символ.
	 */
	char* str;
};

// comparators
inline bool String::operator==(const String& rhs) {
	if (length != rhs.length) {
		return false;
	}
	return std::strcmp(str, rhs.str) ? false : true;
}
inline bool String::operator==(const char* s) {
	return std::strcmp(str, s) ? false : true;
}

// = operator
inline String& String::operator=(const char* s) {
	length = 0;
	delete[] str;
	if (s) {
		length = strlen(s);
		str = new char[length + 1];
		std::strncpy(str, s, length);
	}
	return *this;
}
inline String& String::operator=(const String& rhs) {
	if (this != &rhs) { // check itself object
		length = 0;
		delete[] str;
		if (rhs.str) {
			length = rhs.length;
			str = new char[length + 1];
			std::strncpy(str, rhs.str, length);
		}
	}
	return *this;
}

// [] operator
inline char& String::operator[](int ind) {
	return str[ind];
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
	return os << s.getChars();
}

}

#endif
