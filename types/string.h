#pragma once

namespace php {
	class value;
	class string : public value {
	public:
		string(): value() {}
		string(std::size_t size, bool persistent=false)
			: value(zend_string_alloc(size, persistent), true) {}
		string(const char* val, std::size_t len, bool persistent=false)
			: value(zend_string_init(val, len, persistent), true) {}
		string(const std::string& str)
			: value(zend_string_init(str.c_str(), str.length(), false), true) {}
		inline const char* c_str() const {
			return Z_STRVAL(value_);
		}
		inline char* data() {
			return Z_STRVAL(value_);
		}
		inline operator zend_string*() {
			return Z_STR(value_);
		}
		std::size_t& length() {
			return Z_STRLEN(value_);
		}
		string substr(std::size_t pos, std::size_t count = 0);
		static string concat(const string& s1, const string& s2);
	};
}
