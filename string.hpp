#pragma once

namespace php {
	class string : public value {
	public:
		using value::operator =;
		string() {
			ZVAL_EMPTY_STRING(&value_ins);
		}
		string(const char* str, std::size_t len = -1) {
			ZVAL_STR(&value_ins, zend_string_init(str, (len == -1 ? std::strlen(str) : len), false));
		}
		string(const std::string& str) {
			ZVAL_STR(&value_ins, zend_string_init(str.c_str(), str.length(), false));
		}
		string(buffer&& buf) {
			ZVAL_STR(&value_ins, buf.str_.s);
			buf.str_.s = nullptr;
			buf.str_.a = 0;
		}
		explicit string(std::size_t size) {
			assert(size > 0);
			ZVAL_STR(&value_ins, zend_string_alloc(size, false));
			// 参考 zend_string_init 流程，部分逻辑(例如 JSON)依赖结尾的 \0 字节
			Z_STRVAL(value_ins)[size] = '\0';
		}
		string(zend_string* v)
		: value(v) {
			
		}
		string(const value& v)
		: value(v, TYPE::STRING) {

		}
		string(value&& v)
		: value(std::move(v), TYPE::STRING) {

		}
		// --------------------------------------------------------------------
		const char* c_str() const {
			return Z_STRVAL(value_ins);
		}
		char* data() const {
			return Z_STRVAL(value_ins);
		}
		void shrink(std::size_t length) {
			assert(length < Z_STRLEN(value_ins));
			if (Z_REFCOUNT(value_ins) > 1) {
				Z_DELREF(value_ins);
				ZVAL_STR(&value_ins, zend_string_init(Z_STRVAL(value_ins), length, 0));
			}else{
				Z_STRLEN(value_ins) = length;
				// 参考 zend_string_init 流程，部分逻辑(例如 JSON)依赖结尾的 \0 字节
				Z_STRVAL(value_ins)[length] = '\0';
			}
		}
		string substr(std::size_t pos, std::size_t count = 0) const {
			if(count == 0) count = length() - pos;
			return string(Z_STRVAL(value_ins) + pos, count);
		}
		static string concat(const string& s1, const string& s2) {
			string str(s1.length() + s2.length());
			std::memcpy(Z_STRVAL(str.value_ins)              , s1.c_str(), s1.length());
			std::memcpy(Z_STRVAL(str.value_ins) + s1.length(), s2.c_str(), s2.length());
			Z_STRVAL(str.value_ins)[Z_STRLEN(str.value_ins)] = '\0';
			return std::move(str);
		}
		// --------------------------------------------------------------------
		string& operator =(const value& v) {
			assign(v, TYPE::STRING);
			return *this;
		}
		string& operator =(value&& v) {
			assign(std::move(v), TYPE::STRING);
			return *this;
		}
		string operator +(const string& s) const {
			return concat(*this, s);
		}
		string& operator +=(const string& s) {
			string str = concat(*this, s);
			value::operator =(std::move(str));
			return *this;
		}
		// ------------------------------------------------------------------
		operator std::string() const {
			return std::string(c_str(), size());
		}
	};
}
