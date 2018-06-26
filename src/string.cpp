#include "vendor.h"
#include "string.h"

namespace php {
	string::string() {
		
	}
	string::string(std::nullptr_t n)
	: value(n) {
		
	}
	string::string(const char* str, std::size_t len) {
		ZVAL_STR(&val_, zend_string_init(str, (len == -1 ? std::strlen(str) : len), false));
	}
	string::string(const std::string& str) {
		ZVAL_STR(&val_, zend_string_init(str.c_str(), str.length(), false));
	}
	string::string(buffer&& buf)
	: value(std::move(buf)) {
		
	}
	string::string(stream_buffer&& buf)
	: value(std::move(buf)) {
		
	}
	string::string(const parameter& v)
	: value(v) {

	}
	string::string(const property& v)
	: value(v) {

	}
	string::string(const array_member& v)
	: value(v) {

	}
	string::string(int size)
	:string(std::size_t(size)) {
		
	}
	string::string(std::size_t size) {
		if(size <= 0) {
			ZVAL_EMPTY_STRING(&val_);
		}else{
			ZVAL_STR(&val_, zend_string_alloc(size, false));
			// 参考 zend_string_init 流程，部分逻辑(例如 JSON)依赖结尾的 \0 字节
			Z_STRVAL(val_)[size] = '\0';
		}
	}
	// 注意: 此种构造形式无类型检查
	string::string(zval* v, bool ref)
	: value(v, ref) {
		
	}
	string::string(zend_string* v)
	: value(v) {
		
	}
	string::string(smart_str* v)
	: value(v) {
		
	}
	string::string(const value& v)
	: value(v/*, TYPE::STRING*/) {

	}
	string::string(value&& v)
	: value(std::move(v)/*, TYPE::STRING*/) {

	}
	// --------------------------------------------------------------------
	const char* string::c_str() const {
		return Z_STRVAL_P(ptr_);
	}
	char*string::data() const {
		return Z_STRVAL_P(ptr_);
	}
	void string::shrink(std::size_t length) {
		assert(length < Z_STRLEN_P(ptr_));
		if (Z_REFCOUNT_P(ptr_) > 1) {
			Z_DELREF_P(ptr_);
			ZVAL_STR(ptr_, zend_string_init(Z_STRVAL_P(ptr_), length, 0));
		}else{
			Z_STRLEN_P(ptr_) = length;
			// 参考 zend_string_init 流程，部分逻辑(例如 JSON)依赖结尾的 \0 字节
			Z_STRVAL_P(ptr_)[length] = '\0';
		}
	}
	string string::substr(std::size_t pos, std::size_t count) const {
		if(count == 0) count = length() - pos;
		return string(Z_STRVAL_P(ptr_) + pos, count);
	}
	string string::concat(const string& s1, const string& s2) {
		string str(s1.length() + s2.length());
		std::memcpy(Z_STRVAL_P(str.ptr_)              , s1.c_str(), s1.length());
		std::memcpy(Z_STRVAL_P(str.ptr_) + s1.length(), s2.c_str(), s2.length());
		Z_STRVAL_P(str.ptr_)[Z_STRLEN_P(str.ptr_)] = '\0';
		return std::move(str);
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
}
