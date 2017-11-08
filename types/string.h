#pragma once

namespace php {
	class value;
	class string : public value {
	public:
		string() {
			ZVAL_STR(&value_, ZSTR_EMPTY_ALLOC());
		}
		string(std::nullptr_t np)
			: value(np) {}
		string(std::size_t size)
			: value(zend_string_alloc(size, false), true) {
			// 参考 zend_string_init 流程，部分代码需要依赖结尾的 \0 字节，需要注意
			Z_STRVAL(value_)[size] = '\0';
		}
		string(const string& str): value(str) {}
		string(string&& str): value(std::move(str)) {}
		string(const char* val, std::size_t len)
			: value(zend_string_init(val, len, false), true) {}
		string(const std::string& str)
			: value(zend_string_init(str.c_str(), str.length(), false), true) {}
		string(buffer&& buf)
		 	: value(std::move(buf)) {
		}
		inline const char* c_str() const {
			return Z_STRVAL(value_);
		}
		inline char* data() {
			return Z_STRVAL(value_);
		}
		inline operator zend_string*() {
			return Z_STR(value_);
		}
		std::size_t& length() const {
			return Z_STRLEN(value_);
		}
		string substr(std::size_t pos, std::size_t count = 0);
		static string concat(const string& s1, const string& s2);
		using value::operator =;
		string& operator = (const string& g) {
			value::operator=((const value&)g);
			return *this;
		}
		string& operator = (string&& g) {
			value::operator=(std::move(g));
			return *this;
		}
		string& operator =(php::buffer&& buf);
		using value::operator ==;
		using value::operator std::string;
	};
}
