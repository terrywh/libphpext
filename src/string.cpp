#include "phpext.h"

namespace php {
	string::string() {
		ZVAL_EMPTY_STRING(&value_ins);
	}
	string::string(const char* str, std::size_t len) {
		ZVAL_STR(&value_ins, zend_string_init(str, (len == -1 ? std::strlen(str) : len), false));
	}
	string::string(const std::string& str) {
		ZVAL_STR(&value_ins, zend_string_init(str.c_str(), str.length(), false));
	}
	string::string(buffer&& buf)
	: value(std::move(buf)) {
		
	}
	string::string(int size)
	:string(std::size_t(size)) {
		
	}
	string::string(std::size_t size) {
		assert(size > 0);
		ZVAL_STR(&value_ins, zend_string_alloc(size, false));
		// 参考 zend_string_init 流程，部分逻辑(例如 JSON)依赖结尾的 \0 字节
		Z_STRVAL(value_ins)[size] = '\0';
	}
	// 注意: 此种构造形式无类型检查
	string::string(const zval* v, bool copy)
	: value(v, copy) {
		
	}
	string::string(zend_string* v)
	: value(v) {
		
	}
	string::string(const value& v)
	: value(v, TYPE::STRING) {

	}
	string::string(value&& v)
	: value(std::move(v), TYPE::STRING) {

	}
	// --------------------------------------------------------------------
	const char* string::c_str() const {
		return Z_STRVAL(value_ins);
	}
	char*string::data() const {
		return Z_STRVAL(value_ins);
	}
	void string::shrink(std::size_t length) {
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
	string string::substr(std::size_t pos, std::size_t count) const {
		if(count == 0) count = length() - pos;
		return string(Z_STRVAL(value_ins) + pos, count);
	}
	string string::concat(const string& s1, const string& s2) {
		string str(s1.length() + s2.length());
		std::memcpy(Z_STRVAL(str.value_ins)              , s1.c_str(), s1.length());
		std::memcpy(Z_STRVAL(str.value_ins) + s1.length(), s2.c_str(), s2.length());
		Z_STRVAL(str.value_ins)[Z_STRLEN(str.value_ins)] = '\0';
		return std::move(str);
	}
	// --------------------------------------------------------------------
	string& string::operator =(const value& v) {
		assign(v, TYPE::STRING);
		return *this;
	}
	string& string::operator =(value&& v) {
		assign(std::move(v), TYPE::STRING);
		return *this;
	}
	string& string::operator =(const zval* v) {
		value::operator =(v);
		return *this;
	}
	// -------------------------------------------------------------------
	string string::operator +(const string& s) const {
		return concat(*this, s);
	}
	string string::operator +(const char* s) const {
		return concat(*this, s);
	}
	string& string::operator +=(const string& s) {
		string str = concat(*this, s);
		value::operator =(std::move(str));
		return *this;
	}
	// ------------------------------------------------------------------
	string::operator std::string() const {
		return std::string(c_str(), size());
	}
}
