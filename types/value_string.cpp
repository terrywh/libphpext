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
		assert( is_string() );
		zend_string *target = zend_string_alloc(Z_STRLEN_P(val_) + len, 0),
			*origin = Z_STR_P(val_);

		ZVAL_NEW_STR(val_, target);
		std::memcpy(ZSTR_VAL(target), ZSTR_VAL(origin), ZSTR_LEN(origin));
		std::memcpy(ZSTR_VAL(target) + ZSTR_LEN(origin), str, len);

		zend_string_release(origin);
		return *this;
	}
	value value::substr(int from, int count) {
		assert( is_string() );
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

	bool value::operator==(const char* str) {
		return equal(str, std::strlen(str));
	}
	bool value::operator==(const std::string str) {
		return equal(str.c_str(), str.length());
	}
	bool value::equal(const char* str, std::size_t len) {
		if(ref_ && val_ == nullptr || Z_TYPE_P(val_) != IS_STRING) return false;
		Z_STRLEN_P(val_) == len && std::strncmp(Z_STRVAL_P(val_), str, std::min(Z_STRLEN_P(val_), len));
	}
	bool value::operator>(const char* str) {
        if(!is_string()) throw exception("type error: string expected");
		//assert( is_string() );
		return compare(str, std::strlen(str)) > 0;
	}
	bool value::operator>(const std::string str) {
        if(!is_string()) throw exception("type error: string expected");
		//assert( is_string() );
		return compare(str.c_str(), str.length()) > 0;
	}
	bool value::operator<(const char* str) {
        if(!is_string()) throw exception("type error: string expected");
		//assert( is_string() );
		return compare(str, std::strlen(str)) < 0;
	}
	bool value::operator<(const std::string str) {
        if(!is_string()) throw exception("type error: string expected");
		//assert( is_string() );
		return compare(str.c_str(), str.length()) < 0;
	}
	int value::compare(const char* str, std::size_t len) {
		//assert( is_string() );
        if(!is_string()) throw new exception("type error: string expected");
		if(Z_STRLEN_P(val_) != len) {
			return Z_STRLEN_P(val_) - len;
		}
		return std::strncmp(Z_STRVAL_P(val_), str, std::min(Z_STRLEN_P(val_), len));
	}
	void value::string_length(unsigned int size) {
		Z_STRLEN_P(val_) = size;
	}
}
