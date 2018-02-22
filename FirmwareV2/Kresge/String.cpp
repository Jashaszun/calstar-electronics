#include "String.h"
#include <stdio.h>
#include <stdlib.h>

// substring, concatenation (+), contents char[] that dynamically expands, indexing, cstr
// casting int to string, floats
// string constructor for char*
/* A lightweight version of WString.cpp in the Arduino libraries */

void String::init(const char* cstr) {
	// Copy the contents of cstr into buffer
	_length = strlen(cstr);
	_buffer = (char*)malloc(_length + 1);
	strcpy(_buffer, cstr);
}

// Just make space, don't write
void String::init(size_t len) {
	_length = len;
	_buffer = (char*)malloc(_length + 1);
}

// INTERNAL USE
String::String() { }

String::String(const char* cstr) {
	init(cstr);
}

String::String(const String& value) {
	init(value.buffer());
}

String::String(const char c) {
	char buf[2];
	buf[0] = c;
	buf[1] = 0;
	init(buf);
}

String::String(int value) {
	char buf[12];
	// converts int to string
	sprintf(buf, "%d", value);
	init(buf);
}

String::String(unsigned int value) {
	char buf[12];
	// converts unsigned int to string
	sprintf(buf, "%u", value);
	init(buf);
}

// String::String(long value) {
// 	init();
// }

// String::String(unsigned long value) {
// 	init();
// }

// String::String(float value) {
// 	init();
// }

// String::String(double value) {
// 	init();
// }

String::~String() {
	free(_buffer);
}

int String::length() const {
	return _length;
}

char* String::c_str() {
	return _buffer;
}

const char *String::const_c_str() const {
	return _buffer;
}

const char* String::buffer() const {
	return _buffer;
}

String String::substr(unsigned int start, unsigned int len) {
	String newstr;
	newstr.init(len);
	memcpy(this->_buffer, newstr._buffer, len);
	newstr._buffer[len] = 0;
	return newstr;
}

void String::setCharAt(unsigned int index, char c) {
	_buffer[index] = c;
}

char String::operator[](unsigned int index) const {
	return _buffer[index];
}

String String::operator+ (const String& other) const {
	String newstr;
	newstr.init(_length + other.length());
	memcpy((void*)this->buffer(), newstr.buffer(), _length);
	memcpy((void*)other.buffer(), newstr.buffer() + _length, other.length());
	newstr.c_str()[newstr.length()] = 0;
	return newstr;
}
