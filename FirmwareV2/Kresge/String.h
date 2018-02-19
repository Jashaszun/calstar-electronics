#include "String.h"

/* Mostly copied from WString.h in the Arduino libraries */

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