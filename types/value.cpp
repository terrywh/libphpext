#include "../phpext.h"

namespace php {
	// 字符串
	// -------------------------------------------------------------------------
	value::value(buffer&& buf) {
		smart_str_0(&buf.str_); // 添加 \0 结束符（缺少时可能导致 JSON 解析失败）
		ZVAL_STR(&value_, buf.str_.s);
		buf.str_.s = nullptr;
		// buf.str_.a = 0;
	}
	// 对象
	// ---------------------------------------------------------------------
	value::value(class_base* base) {
		if(Z_TYPE(base->value_) != IS_UNDEF) {
			ZVAL_COPY(&value_, &base->value_);
		}else{
			assert("object is not yet set");
		}
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
				return zend_hash_num_elements(Z_ARRVAL(value_));
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
	value::operator zval*() {
		return &value_;
	}
	value::operator int() const {
		switch(Z_TYPE(value_)) {
		case IS_TRUE:
			return 1;
		case IS_LONG:
			return Z_LVAL(value_);
		case IS_DOUBLE:
			zend_error_noreturn(E_USER_NOTICE, "type of %s expected, %s given", zend_get_type_by_const(IS_LONG), zend_get_type_by_const(IS_DOUBLE));
			return Z_DVAL(value_);
		default: // TODO 其它类型？
			zend_error_noreturn(E_USER_WARNING, "type of %s expected, %s given", zend_get_type_by_const(IS_LONG), zend_get_type_by_const(Z_TYPE(value_)));
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
			zend_error_noreturn(E_USER_NOTICE, "type of %s expected, %s given", zend_get_type_by_const(IS_LONG), zend_get_type_by_const(IS_DOUBLE));
			return Z_DVAL(value_);
		default: // TODO 其它类型？
			zend_error_noreturn(E_USER_WARNING, "type of %s expected, %s given", zend_get_type_by_const(IS_LONG), zend_get_type_by_const(Z_TYPE(value_)));
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
			zend_error_noreturn(E_USER_NOTICE, "type of %s expected, %s given", zend_get_type_by_const(IS_LONG), zend_get_type_by_const(IS_DOUBLE));
			return Z_DVAL(value_);
		default: // TODO 其它类型？
			zend_error_noreturn(E_USER_WARNING, "type of %s expected, %s given", zend_get_type_by_const(IS_LONG), zend_get_type_by_const(Z_TYPE(value_)));
			return 0l;
		}
	}
	value::operator double() const {
		switch(Z_TYPE(value_)) {
		case IS_FALSE:
			zend_error_noreturn(E_USER_NOTICE, "type of %s expected, %s given",
				zend_get_type_by_const(IS_DOUBLE), zend_get_type_by_const(IS_FALSE));
			return 0.;
		case IS_TRUE:
			zend_error_noreturn(E_USER_NOTICE, "type of %s expected, %s given",
				zend_get_type_by_const(IS_DOUBLE), zend_get_type_by_const(IS_TRUE));
			return 1.;
		case IS_LONG:
			zend_error_noreturn(E_USER_NOTICE, "type of %s expected, %s given",
				zend_get_type_by_const(IS_DOUBLE), zend_get_type_by_const(IS_LONG));
			return Z_LVAL(value_);
		case IS_DOUBLE:
			return Z_DVAL(value_);
		default: // TODO 其它类型？
			zend_error_noreturn(E_USER_WARNING, "type of %s expected, %s given",
				zend_get_type_by_const(IS_DOUBLE), zend_get_type_by_const(Z_TYPE(value_)));
			return 0.;
		}
	}
	value::operator std::string() const {
		switch(Z_TYPE(value_)) {
		case IS_STRING:
			return std::string(Z_STRVAL(value_), Z_STRLEN(value_));
		default:
			zend_error_noreturn(E_USER_WARNING, "type of %s expected, %s given",
				zend_get_type_by_const(IS_STRING), zend_get_type_by_const(Z_TYPE(value_)));
			return std::string();
		}
	}
	value::operator zend_string*() {
		switch(Z_TYPE(value_)) {
		case IS_STRING:
			return Z_STR(value_);
		default:
			zend_error_noreturn(E_USER_WARNING, "type of %s expected, %s given",
				zend_get_type_by_const(IS_STRING), zend_get_type_by_const(Z_TYPE(value_)));
			return nullptr;
		}
	}
	value::operator string&() {
		if(is_string()) {
			return *reinterpret_cast<string*>(&value_);
		}
		throw exception("type error: string expected");
	}
	value::operator zend_array*() {
		switch(Z_TYPE(value_)) {
		case IS_ARRAY:
			return Z_ARRVAL(value_);
		default:
			zend_error_noreturn(E_USER_WARNING, "type of %s expected, %s given",
				zend_get_type_by_const(IS_ARRAY), zend_get_type_by_const(Z_TYPE(value_)));
			return nullptr;
		}
	}
	value::operator array&() {
		if(is_array()) {
			return *reinterpret_cast<array*>(&value_);
		}
		throw exception("type error: array expected");
	}
	value::operator zend_object*() {
		switch(Z_TYPE(value_)) {
		case IS_OBJECT:
			return Z_OBJ(value_);
		default:
			zend_error_noreturn(E_USER_WARNING, "type of %s expected, %s given",
				zend_get_type_by_const(IS_OBJECT), zend_get_type_by_const(Z_TYPE(value_)));
			return nullptr;
		}
	}
	value::operator object&() {
		if(is_object()) {
			return *reinterpret_cast<object*>(&value_);
		}
		throw exception("type error: object expected");
	}
	value::operator callable&() {
		if(is_callable()) {
			return *reinterpret_cast<callable*>(&value_);
		}
		throw exception("type error: callable expected");
	}
	value::operator zend_refcounted*() {
		if(Z_REFCOUNTED(value_)) return Z_COUNTED(value_);
		zend_error_noreturn(E_USER_WARNING, "recounted expected, %s given",
			zend_get_type_by_const(Z_TYPE(value_)));
		return nullptr;
	}
	value::operator zend_generator*() {
		if(is_generator())
			return reinterpret_cast<zend_generator*>(Z_OBJ(value_));
		return nullptr;
	}
	value::operator generator&() {
		if(is_generator())
			return *reinterpret_cast<generator*>(&value_);
		throw exception("type error: instance of 'Generator' expected");
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
	value& value::operator =(std::nullptr_t np) {
		_zval_dtor(&value_);
		ZVAL_NULL(&value_);
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
