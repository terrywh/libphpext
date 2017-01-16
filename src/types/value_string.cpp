#include "value.h"

namespace php
{
	value::value(const char* str):value() {
		ZVAL_NEW_STR(val_, zend_string_init(str, std::strlen(str), false));
	}
	value::value(const char* str, std::size_t len):value() {
		ZVAL_NEW_STR(val_, zend_string_init(str, len, false));
	}
	value::value(const std::string& str):value() {
		ZVAL_NEW_STR(val_, zend_string_init(str.c_str(), str.length(), false));
	}
	value& value::to_string() {
		_convert_to_string(val_); 	// 强制转换
		return *this;
	}
	value::operator const char*() {
		assert(Z_TYPE_P(val_) == IS_STRING);
		return Z_STRVAL_P(val_);
	}
	value& value::operator= (const char* str) {
		return assign_(str, std::strlen(str));
	}
	value& value::operator= (const std::string& str) {
		return assign_(str.c_str(), str.length());
	}
	value& value::assign_(const char* str, std::size_t len) {
		assert(Z_TYPE_P(val_) == IS_STRING);
		_zval_dtor(val_);
		ZVAL_NEW_STR(val_, zend_string_init(str, len, false));
		return *this;
	}
	value& value::operator+=(const char* str) {
		return append_(str, std::strlen(str));
	}
	value& value::operator+=(const std::string& str) {
		return append_(str.c_str(), str.length());
	}
	value& value::append_(const char* str, std::size_t len) {
		assert(Z_TYPE_P(val_) == IS_STRING);
		zend_string *target = zend_string_alloc(Z_STRLEN_P(val_) + len, 0),
			*origin = Z_STR_P(val_);

		ZVAL_NEW_STR(val_, target);
		std::memcpy(ZSTR_VAL(target), ZSTR_VAL(origin), ZSTR_LEN(origin));
		std::memcpy(ZSTR_VAL(target) + ZSTR_LEN(origin), str, len);

		zend_string_release(origin);
		return *this;
	}

	value value::substr(int from, int count) {
		if(count == 0) {
			count = Z_STRLEN_P(val_);
		}
		if(from >= 0 && count > 0) {
			count = std::min((int)Z_STRLEN_P(val_) - from, count);
		}else if(from >= 0 && count < 0) {
			count = Z_STRLEN_P(val_) - from;
		}else if(from < 0 && count > 0) {
			from = Z_STRLEN_P(val_) + from;
			count = std::min(-from, count);
		}else if(from < 0 && count < 0) {
			from = Z_STRLEN_P(val_) + from;
			count = std::min(-from, count - from);
		}else{
			return value(nullptr);
		}
		return value(Z_STRVAL_P(val_) + from, count);
	}
	// TODO 判定
}
