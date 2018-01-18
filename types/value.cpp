#include "../phpext.h"

namespace php {
	php::value BOOL_YES;
	php::value BOOL_NO;
	// 字符串
	// -------------------------------------------------------------------------
	value::value(buffer&& buf) {
		smart_str_0(&buf.str_); // 添加 \0 结束符（缺少时可能导致 JSON 解析失败）
		ZVAL_STR(&value_, buf.str_.s);
		buf.str_.s = nullptr;
		buf.str_.a = 0;
	}
	// 对象
	// ---------------------------------------------------------------------
	value::value(class_base* base) {
		if(base != nullptr) ZVAL_COPY(&value_, &base->value_);
		else ZVAL_NULL(&value_);
	}
	// 闭包
	// ---------------------------------------------------------------------
	value::value(std::function<value (parameters&)> fn) {
		ZVAL_OBJ(&value_, class_entry<class_closure>::create_object());
		class_wrapper<class_closure>::from_obj(Z_OBJ(value_))->fn_ = fn;
	}
	// -------------------------------------------------------------------------
	bool value::is_empty() const {
		switch(Z_TYPE(value_)) {
			case IS_UNDEF:
			case IS_NULL:
			case IS_FALSE:
				return true;
			case IS_LONG:
			case IS_DOUBLE:
				return Z_LVAL(value_) == 0;
			case IS_STRING:
				return Z_STRLEN(value_) == 0;
			case IS_ARRAY:
				return Z_ARRVAL(value_)->nNumOfElements == 0;
			default: // TODO how to determine 'empty' for other types?
				return false;
		}
	}
	bool value::is_undefined() const {
		return Z_TYPE(value_) == IS_UNDEF;
	}
	bool value::is_null() const {
		switch(Z_TYPE(value_)) {
			case IS_UNDEF:
			case IS_NULL:
				return true;
			default:
				return false;
		}
	}
	std::size_t value::length() const {
		switch(Z_TYPE(value_)) {
			case IS_UNDEF:
			case IS_NULL:
				return 0l;
			case IS_FALSE:
			case IS_TRUE:
				return sizeof(zend_bool);
			case IS_LONG:
				return sizeof(zend_long);
			case IS_DOUBLE:
				return sizeof(double);
			case IS_STRING:
				return Z_STRLEN(value_);
			case IS_ARRAY:
				return zend_array_count(Z_ARRVAL(value_));
			default: // TODO 其它类型？
				return -1;
		}
	}
	bool value::is_true() const {
		switch(Z_TYPE(value_)) {
		case IS_UNDEF:
		case IS_NULL:
		case IS_FALSE:
			return false;
		case IS_TRUE:
			return sizeof(zend_bool);
		case IS_LONG:
			return Z_LVAL(value_) != 0;
		case IS_DOUBLE:
			return Z_DVAL(value_) != 0;
		case IS_STRING:
			return Z_STRLEN(value_) > 0;
		case IS_ARRAY:
			return zend_hash_num_elements(Z_ARRVAL(value_)) > 0;
		default: // TODO 其它类型？
			return true;
		}
	}
	value::operator zval*() const {
		return const_cast<zval*>(&value_);
	}
	value::operator int() const {
		switch(Z_TYPE(value_)) {
		case IS_TRUE:
			return 1;
		case IS_LONG:
			return Z_LVAL(value_);
		case IS_DOUBLE:
			return Z_DVAL(value_);
		default: // TODO 其它类型？
			return 0;
		}
	}
	value::operator std::int64_t() const {
		switch(Z_TYPE(value_)) {
		case IS_TRUE:
			return 1l;
		case IS_LONG:
			return Z_LVAL(value_);
		case IS_DOUBLE:
			return Z_DVAL(value_);
		default: // TODO 其它类型？
			return 0l;
		}
	}
	value::operator std::size_t() const {
		switch(Z_TYPE(value_)) {
		case IS_TRUE:
			return 1l;
		case IS_LONG:
			return Z_LVAL(value_);
		case IS_DOUBLE:
			return Z_DVAL(value_);
		default: // TODO 其它类型？
			return 0l;
		}
	}
	value::operator double() const {
		switch(Z_TYPE(value_)) {
		case IS_FALSE:
			return 0.;
		case IS_TRUE:
			return 1.;
		case IS_LONG:
			return Z_LVAL(value_);
		case IS_DOUBLE:
			return Z_DVAL(value_);
		default: // TODO 其它类型？
			return 0.;
		}
	}
	value::operator std::string() const {
		return std::string(Z_STRVAL(value_), Z_STRLEN(value_));
	}
	value::operator zend_string*() const {
		return Z_STR(value_);
	}
	value::operator zend_array*() const {
		return Z_ARRVAL(value_);
	}
	value::operator zend_object*() const {
		return Z_OBJ(value_);
	}
	value::operator zend_refcounted*() const {
		return Z_COUNTED(value_);
	}
	value::operator zend_generator*() const {
		return reinterpret_cast<zend_generator*>(Z_OBJ(value_));
	}
	value::operator string&() const {
		return (string&)*this;
	}
	value::operator array&() const {
		return (array&)*this;
	}
	value::operator object&() const {
		return (object&)*this;
	}
	value::operator callable&() const {
		return (callable&)*this;
	}
	value::operator generator&() const {
		return (generator&)*this;
	}
	value& value::operator=(const value& v) {
		_zval_dtor(&value_);
		ZVAL_COPY(&value_, &v.value_);
		return *this;
	}
	value& value::operator=(value&& v) {
		_zval_dtor(&value_);
		ZVAL_COPY_VALUE(&value_, &v.value_);
		ZVAL_UNDEF(&v.value_);
		return *this;
	}
	value& value::operator =(buffer&& buf) {
		_zval_dtor(&value_);
		smart_str_0(&buf.str_); // 添加 \0 结束符（缺少时可能导致 JSON 解析失败）
		ZVAL_STR(&value_, buf.str_.s);
		buf.str_.s = nullptr;
		buf.str_.a = 0;
		return *this;
	}
	value& value::operator =(std::nullptr_t np) {
		_zval_dtor(&value_);
		ZVAL_NULL(&value_);
		return *this;
	}
	bool value::operator==(const value& v) {
		return Z_PTR(value_) == Z_PTR(v.value_);
	}
	bool value::to_bool() {
		convert_to_boolean(&value_);
		return Z_TYPE(value_) == IS_TRUE;
	}
	long value::to_long(int base) {
		convert_to_long_base(&value_, base);
		return Z_LVAL(value_);
	}
	double value::to_double() {
		convert_to_double(&value_);
		return Z_DVAL(value_);
	}
	php::string& value::to_string() {
		convert_to_string(&value_);
		return *reinterpret_cast<string*>(&value_);
	}
}
