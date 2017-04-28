#include "../phpext.h"

namespace php {
	value::~value() {
		_zval_dtor(&value_);
	}
	value::value() {
		ZVAL_UNDEF(&value_);
	}
	value::value(std::nullptr_t v) {
		ZVAL_NULL(&value_);
	}
	value::value(const zval& v) {
		ZVAL_COPY(&value_, &v);
	}
	value::value(zval&& v) {
		ZVAL_COPY_VALUE(&value_, &v);
		ZVAL_UNDEF(&v);
	}
	value::value(const value& w) {
		ZVAL_COPY(&value_, &w.value_);
	}
	value::value(value&& w) {
		ZVAL_COPY_VALUE(&value_, &w.value_);
		ZVAL_UNDEF(&w.value_);
	}
	value::value(zend_string* str) {
		ZVAL_STR(&value_, str);
		zval_addref_p(&value_);
	}
	value::value(const string& str) {
		ZVAL_STR(&value_, str.str_);
		zval_addref_p(&value_);
	}
	value::value(string&& str) {
		ZVAL_STR(&value_, str.str_);
		str.str_ = nullptr;
	}
	value::value(buffer&& buf) {
		ZVAL_STR(&value_, buf.str_.s);
		buf.str_.s = nullptr;
		buf.str_.a = 0;
		buf.po_ = 0;
	}
	value::value(zend_array* arr) {
		ZVAL_ARR(&value_, arr);
		zval_addref_p(&value_);
	}
	value::value(const array& arr) {
		ZVAL_ARR(&value_, arr.arr_);
		zval_addref_p(&value_);
	}
	value::value(array&& arr) {
		ZVAL_ARR(&value_, arr.arr_);
		arr.arr_ = nullptr;
	}
	value::value(zend_object* obj) {
		ZVAL_OBJ(&value_, obj);
		zval_addref_p(&value_);
	}
	value::value(const object& obj) {
		ZVAL_OBJ(&value_, obj.obj_);
		zval_addref_p(&value_);
	}
	value::value(object&& obj) {
		ZVAL_OBJ(&value_, obj.obj_);
		obj.obj_ = nullptr;
	}
	value::value(bool v) {
		if(v) ZVAL_TRUE(&value_);
		else ZVAL_FALSE(&value_);
	}
	value::value(int v) {
		ZVAL_LONG(&value_, v);
	}
	value::value(std::int64_t v) {
		ZVAL_LONG(&value_, v);
	}
	value::value(double v) {
		ZVAL_DOUBLE(&value_, v);
	}
	value::value(const std::string& str) {
		ZVAL_NEW_STR(&value_, zend_string_init(str.c_str(), str.length(), false));
	}
	value::value(const char* str, std::size_t len, bool p) {
		ZVAL_NEW_STR(&value_, zend_string_init(str, len, p));
	}
	value::value(class_base* base) {
		if(Z_TYPE(base->value_) != IS_UNDEF) {
			ZVAL_COPY(&value_, &base->value_);
		}else{
			assert("object is not yet set");
		}
	}
	value::value(std::function<value (parameters&)> fn) {
		ZVAL_OBJ(&value_, class_entry<class_closure>::create_object());
		class_closure* c = native<class_closure>();
		c->fn_ = fn;
	}
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
	bool value::is_instance_of(const std::string& class_name) const {
		if(Z_TYPE(value_) != IS_OBJECT) return false;
		zend_string*      cn = zend_string_init(class_name.c_str(), class_name.length(), false);
		zend_class_entry* ce = zend_lookup_class(cn);
		zend_string_release(cn);
		return instanceof_function(Z_OBJCE(value_), ce);
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
	value::operator string() {
		return Z_STR(value_);
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
	value::operator array() {
		return Z_ARRVAL(value_);
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
	value::operator object() {
		return Z_OBJ(value_);
	}
	value::operator callable() {
		return &value_;
	}
	value& value::operator=(const value& v) {
		_zval_dtor(&value_);
		ZVAL_COPY(&value_, &v.value_);
	}
	value& value::operator=(value&& v) {
		_zval_dtor(&value_);
		ZVAL_COPY_VALUE(&value_, &v.value_);
		ZVAL_UNDEF(&v.value_);
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
	zend_string* value::to_string() {
		convert_to_string(&value_);
		return Z_STR(value_);
	}
}
