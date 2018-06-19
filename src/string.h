#pragma once

namespace php {
	class parameter;
	class string : public value {
	public:
		string();
		string(const char* str, std::size_t len = -1);
		string(const std::string& str);
		string(buffer&& buf);
		explicit string(int size);
		explicit string(std::size_t size);
		string(zval* v, bool ref = false);
		string(zend_string* v);
		string(const value& v);
		string(value&& v);
		// --------------------------------------------------------------------
		const char* c_str() const;
		char* data() const;
		void shrink(std::size_t length);
		string substr(std::size_t pos, std::size_t count = 0) const;
		static string concat(const string& s1, const string& s2);
		// --------------------------------------------------------------------
		string& operator =(const value& v);
		string& operator =(value&& v);
		string operator +(const string& s) const;
		string operator +(const char* s) const;
		string& operator +=(const string& s);
		// ------------------------------------------------------------------
		using value::operator =;
	};
}