#pragma once

namespace php {
	class value;
	class string : public value {
	public:
		string(): value() {}
		string(std::size_t size, bool persistent=false)
			: value(zend_string_alloc(size, persistent), true) {
			// 参考 zend_string_init 流程，部分代码需要依赖结尾的 \0 字节，需要注意
			Z_STRVAL(value_)[size] = '\0';
		}
		string(const char* val, std::size_t len, bool persistent=false)
			: value(zend_string_init(val, len, persistent), true) {}
		string(const std::string& str)
			: value(zend_string_init(str.c_str(), str.length(), false), true) {}
		string(buffer&& buf) {
			ZVAL_STR(&value_, buf);
			buf.str_.s = nullptr;
			buf.str_.a = 0;
			buf.po_ = 0;
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
		using value::operator ==;
	};
}
