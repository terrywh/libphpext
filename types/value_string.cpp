#include "../phpext.h"

namespace php
{
	value::value(const char* str)
	: value(str, std::strlen(str), false) {

	}
	value::value(const std::string& str)
	: value(str.c_str(), str.length(), false) {

	}

	value::value(const char* str, std::size_t len, bool persistent)
	: val_(&value_)
	, ref_(false) {
		ZVAL_NEW_STR(val_, zend_string_init(str, len, persistent));
	}
	value::value(buffer&& buf)
	: val_(&value_)
	, ref_(false) {
		ZVAL_NEW_STR(val_, buf.str_.s);
		buf.str_.s = nullptr;
	}

	value::value(const buffer& buf)
	: val_(&value_)
	, ref_(false) {
		ZVAL_STR_COPY(val_, buf.str_.s);
	}

	value value::string(int size) {
		value v;
		ZVAL_NEW_STR(v.val_, zend_string_alloc(size, false));
		return std::move(v);
	}
	value& value::to_string() {
		_convert_to_string(val_); 	// 强制转换
		return *this;
	}
	value::operator zend_string*() {
//		assert( (!ref_ || val_ != nullptr) && Z_TYPE_P(val_) == IS_STRING );
        if(!is_string()) throw exception("type error: string expected");
		return Z_STR_P(val_);
	}
	value::operator const char*() {
//		assert( (!ref_ || val_ != nullptr) && Z_TYPE_P(val_) == IS_STRING );
        if(!is_string()) throw exception("type error: string expected");
		return Z_STRVAL_P(val_);
	}
	// TODO 判定
	value& value::operator+=(const char* str) {
		return append_(str, std::strlen(str));
	}
	value& value::operator+=(const std::string& str) {
		return append_(str.c_str(), str.length());
	}
	value& value::append_(const char* str, std::size_t len) {
		if( is_string() ) throw exception("type error: string expected");
		zend_string *target = zend_string_alloc(Z_STRLEN_P(val_) + len, 0),
			*origin = Z_STR_P(val_);

		ZVAL_NEW_STR(val_, target);
		std::memcpy(ZSTR_VAL(target), ZSTR_VAL(origin), ZSTR_LEN(origin));
		std::memcpy(ZSTR_VAL(target) + ZSTR_LEN(origin), str, len);

		zend_string_release(origin);
		return *this;
	}
	value value::substr(int from, int count) {
		if( is_string() ) throw exception("type error: string expected");
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

		return value(Z_STRVAL_P(val_) + from, count, GC_FLAGS(Z_STR_P(val_)) & IS_STR_PERSISTENT);
	}
}
