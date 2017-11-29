#pragma once

namespace php {
	class value;
	class string : public value {
	public:
		string():value() {}
		explicit string(std::nullptr_t np)
			: value(np) {}
		explicit string(int size): string(std::size_t(size)) {}
		explicit string(std::size_t size) {
			if(size > 0) {
				// 参考 zend_string_init 流程，部分代码需要依赖结尾的 \0 字节，需要注意
				ZVAL_STR(&value_, zend_string_alloc(size, false));
				Z_STRVAL(value_)[size] = '\0';
			}else{
				ZVAL_STR(&value_, ZSTR_EMPTY_ALLOC());
			}
		}
		explicit string(const std::string& str)
			: value(zend_string_init(str.c_str(), str.length(), false), true) {}
		string(buffer&& buf)
		 	: value(std::move(buf)) {
		}
		string(const string& str): value(str) {}
		string(string&& str): value(std::move(str)) {}
		string(const char* val, std::size_t len)
			: value(zend_string_init(val, len, false), true) {}
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
		using value::operator ==;
		using value::operator =;
		inline string& operator=(const string& v) {
			value::operator=(v);
			return *this;
		}
		inline string& operator=(string&& v) {
			value::operator=(std::move(v));
			return *this;
		}
		string& operator =(php::buffer&& buf);
		using value::operator std::string;
	};
}
