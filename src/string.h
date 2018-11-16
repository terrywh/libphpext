#pragma once

#include "value.h"

namespace php {
	class buffer;
	class stream_buffer;
	class parameter;
	class property;
	class array_member;
	class string : public value {
	public:
		string(); // undefined
		string(std::nullptr_t n);
		string(const char* str, std::size_t len = -1);
		string(const std::string& str);
		string(buffer&& buf);
		string(stream_buffer&& buf);
		string(const parameter& v);
		string(const property& v);
		string(const array_member& v);
		explicit string(int size);
		explicit string(std::size_t size);
		string(zval* v, bool ref = false);
		string(zend_string* v);
		string(smart_str* v);
		string(const value& v);
		string(value&& v);
		// --------------------------------------------------------------------
		const char* c_str() const;
		char* data() const;
		void shrink(std::size_t length);
		string substr(std::size_t pos, std::size_t count = 0) const;
		static string concat(const string& s1, const string& s2);
		// --------------------------------------------------------------------
		string operator +(const string& s) const;
		string operator +(const char* s) const;
		bool   operator <(const string& s) const;
		bool   operator ==(const string& s) const;
		string& operator +=(const string& s);
		// ------------------------------------------------------------------
		using value::operator =;
	};
}
