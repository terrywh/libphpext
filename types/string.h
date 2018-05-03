#pragma once

namespace php {
	class value;
	class string : public value {
	public:
		using value::value;
		using value::operator ==;
		using value::operator =;
		using value::operator zval*;
		using value::operator zend_string*;
		string(): value() {}
		explicit string(int size): string(std::size_t(size)) {}
		explicit string(std::size_t size) {
			if(size > 0) {
				// 参考 zend_string_init 流程，部分代码需要依赖结尾的 \0 字节，需要注意
				ZVAL_STR(&value_, zend_string_alloc(size, false));
				Z_STRVAL(value_)[size] = '\0';
			}else{
				ZVAL_EMPTY_STRING(&value_);
			}
		}
		string(const php::value& v): value(v) {}
		string(php::value&& v): value(std::move(v)) {}
		
		inline const char* c_str() const {
			return Z_STRVAL(value_);
		}
		inline char* data() {
			return Z_STRVAL(value_);
		}
		void resize(std::size_t length) {
			Z_STRLEN(value_) = length;
			Z_STRVAL(value_)[length] = '\0';
		}
		string substr(std::size_t pos, std::size_t count = 0);
		static string concat(const string& s1, const string& s2);
	};
}
