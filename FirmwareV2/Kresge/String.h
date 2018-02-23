#ifndef _KRESGE_STRING_H_
#define _KRESGE_STRING_H_

#include <stddef.h>
#include <string.h>

/* Mostly copied from WString.h in the Arduino libraries */

class String {

public:
  // Constructors
  String(const char *cstr);
  String(const String& value);
  String(const char c);
  String(int value);
  String(unsigned int value);
  // String(long value);
  // String(unsigned long value);
  // String(float value);
  // String(double value);
  ~String();

  int length() const;
  char* c_str();
  const char *const_c_str() const;

  String substr(unsigned int start, unsigned int len);
  // char charAt(unsigned int index) const;
  void setCharAt(unsigned int index, char c);
  char operator[] (unsigned int index) const;
  // char& operator [] (unsigned int index);

  // int compareTo(const String &s) const;
  // bool equals(const String &s) const;
  // bool equals(const char *cstr) const;
  bool operator== (const String &rhs) const;
  // bool operator== (const char *cstr) const {return equals(cstr);}
  bool operator!= (const String &rhs) const;
  // bool operator!= (const char *cstr) const {return !equals(cstr);}
  // bool operator<  (const String &rhs) const;
  // bool operator>  (const String &rhs) const;
  // bool operator<= (const String &rhs) const;
  // bool operator>= (const String &rhs) const;
  // bool equalsIgnoreCase(const String &s) const;
  // bool startsWith( const String &prefix) const;
  // bool startsWith(const String &prefix, unsigned int offset) const;
  // bool endsWith(const String &suffix) const;

  String operator+ (const String& other) const;

private:
  unsigned int _length; // should be buffer length - 1, because of null terminator
  char* _buffer;
  String(); // INTERNAL USE ONLY
  void init(const char *cstr);
  void init(size_t len);
  const char* buffer() const;

};

#endif