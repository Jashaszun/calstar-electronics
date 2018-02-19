#include "String.h"

// substring, concatenation (+), contents char[] that dynamically expands, indexing, cstr
// casting int to string, floats
// string constructor for char*
/* A lightweight version of WString.cpp in the Arduino libraries */

inline void String::init() {
	buffer = NULL;
	length = 0;
}

String::String(const char* cstr) {
	init();
	// Copy the contents of cstr into buffer
	strcpy(buffer, cstr);
}

String::String(const String& value) {
	init();
	*this = value;
}

String::String(const char c) {
	init();
	char buf[2];
	buf[0] = c;
	buf[1] = 0;
	buffer = buf;
	// do i need to free anything?
}

String::String(int value) {
	init();
	char buf[2 + 8 * sizeof(int)];
	// converts int to string
	itoa(value, buf, 10);
	buffer = buf;
}

String::String(unsigned int value) {
	init();
}

String::String(long value) {
	init();
}

String::String(unsigned long value) {
	init();
}

String::String(float value) {
	init();
}

String::String(double value) {
	init();
}

String::~String() {
	init();
}

class String {

public:
	// Constructors
	String(const char *cstr);
	String(const String& value);
	String(const char c);
	String(int value);
	String(unsigned int value);
	String(long value);
	String(unsigned long value);
	String(float value);
	String(double value);
	~String();

	int length();

	String& substring(unsigned int start, unsigned int end);
	char charAt(unsigned int index) const;
	void setCharAt(unsigned int index, char c);
	char operator [] (unsigned int index) const;
	char& operator [] (unsigned int index);

	int compareTo(const String &s) const;
	bool equals(const String &s) const;
	bool equals(const char *cstr) const;
	bool operator == (const String &rhs) const {return equals(rhs);}
	bool operator == (const char *cstr) const {return equals(cstr);}
	bool operator != (const String &rhs) const {return !equals(rhs);}
	bool operator != (const char *cstr) const {return !equals(cstr);}
	bool operator <  (const String &rhs) const;
	bool operator >  (const String &rhs) const;
	bool operator <= (const String &rhs) const;
	bool operator >= (const String &rhs) const;
	bool equalsIgnoreCase(const String &s) const;
	bool startsWith( const String &prefix) const;
	bool startsWith(const String &prefix, unsigned int offset) const;
	bool endsWith(const String &suffix) const;

	String& operator + (const String& left, const String& right);

private:
	int length; // should be buffer length - 1, because of null terminator
	char* buffer;
	void init();

}