#include "phpext.h"

namespace php {
	// ---------------------------------------------------------------------
	value::~value() {
		zval_ptr_dtor(&value_ins);
	}
	// ---------------------------------------------------------------------
	value::value() {
		ZVAL_UNDEF(&value_ins);
	}
	value::value(std::nullptr_t v) {
		ZVAL_NULL(&value_ins);
	}
	value::value(class_base* v)
	: value(&v->obj_) {}
	value::value(const zval* v) {
		if(v == nullptr) {
			ZVAL_UNDEF(&value_ins);
		}else{
			ZVAL_COPY(&value_ins, v);
		}
	}
	value::value(const zend_string* v) {
		ZVAL_STR(&value_ins, const_cast<zend_string*>(v));
		addref();
	}
	value::value(const zend_object* v) {
		ZVAL_OBJ(&value_ins, const_cast<zend_object*>(v));
		addref();
	}
	value::value(zend_class_entry* e) {
		int r = object_init_ex(&value_ins, e);
		assert(r == SUCCESS && "无法创建实例");
		object::call(&value_ins, "__construct");
	}
	value::value(const CLASS& c)
	: value(static_cast<zend_class_entry*>(c)) {
		
	}
	value::value(zend_class_entry* e, std::vector<value> argv) {
		int r = object_init_ex(&value_ins, e);
		assert(r == SUCCESS && "无法创建实例");
		object::call(&value_ins, "__construct", std::move(argv));
	}
	value::value(const CLASS& c, std::vector<value> argv)
	: value(static_cast<zend_class_entry*>(c), std::move(argv)) {

	}
	value::value(const zend_array* v) {
		ZVAL_ARR(&value_ins, const_cast<zend_array*>(v));
		addref();
	}
	value::value(const void* data) {
		ZVAL_PTR(&value_ins, const_cast<void*>(data));
	}
	value::value(const value& w) {
		ZVAL_COPY(&value_ins, &w.value_ins);
	}
	value::value(value&& w) {
		ZVAL_COPY_VALUE(&value_ins, &w.value_ins);
		ZVAL_UNDEF(&w.value_ins);	
	}
	// 类型检查构造
	value::value(const value& v, const TYPE& t) {
		if(!v.typeof(t)) {
			throw exception(zend_ce_type_error, "type of '" + t.name() + "' is expected, '" + v.typeof().name() + "' given");
		}
		ZVAL_COPY(&value_ins, v);
	}
	value::value(value&& v, const TYPE& t) {
		if(!v.typeof(t)) {
			throw exception(zend_ce_type_error, "type of '" + t.name() + "' is expected, '" + v.typeof().name() + "' given");
		}
		ZVAL_COPY_VALUE(&value_ins, &v.value_ins);
		ZVAL_UNDEF(&v.value_ins);
	}
	value::value(const value& v, const CLASS& c) {
		if(!v.instanceof(c)) throw exception(zend_ce_type_error, "instance of '" + c.name() + "' is expected, '" + v.classof().name() + "' given");
		ZVAL_COPY(&value_ins, v);
	}
	value::value(value&& v, const CLASS& c) {
		if(!v.instanceof(c)) throw exception(zend_ce_type_error, "instance of '" + c.name() + "' is expected, '" + v.classof().name() + "' given");
		ZVAL_COPY_VALUE(&value_ins, &v.value_ins);
		ZVAL_UNDEF(&v.value_ins);
	}
	// 基础类型
	// ---------------------------------------------------------------------
	value::value(bool v) {
		ZVAL_BOOL(&value_ins, v);
	}
	value::value(int v) {
		ZVAL_LONG(&value_ins, v);
	}
	value::value(std::uint32_t v) {
		ZVAL_LONG(&value_ins, v);
	}
	value::value(std::int64_t v) {
		ZVAL_LONG(&value_ins, v);
	}
	value::value(std::size_t v) {
		ZVAL_LONG(&value_ins, v);
	}
	value::value(double v) {
		ZVAL_DOUBLE(&value_ins, v);
	}
	value::value(const char* str) {
		ZVAL_STRINGL(&value_ins, str, std::strlen(str));
	}
	value::value(const std::string& str) {
		ZVAL_STRINGL(&value_ins, str.c_str(), str.length());
	}
	value::value(std::function<value (parameters& params)> fn) {
		int r = object_init_ex(&value_ins, class_entry<closure>::entry());
		assert(r == SUCCESS && "无法创建实例");
		from_object<closure>(&value_ins)->fn_ = fn;
	}
	// ---------------------------------------------------------------------
	bool value::empty() const {
		switch(Z_TYPE(value_ins)) {
		case IS_UNDEF:
		case IS_NULL:
		case IS_FALSE:
			return true;
		case IS_LONG:
		case IS_DOUBLE:
			return Z_LVAL(value_ins) == 0;
		case IS_STRING:
			return Z_STRLEN(value_ins) == 0;
		case IS_ARRAY:
			return Z_ARRVAL(value_ins)->nNumOfElements == 0;
		default: // TODO how to determine 'empty' for other types?
			return false;
		}
	}
	std::size_t value::length() const {
		switch(Z_TYPE(value_ins)) {
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
				return Z_STRLEN(value_ins);
			case IS_ARRAY:
				return zend_array_count(Z_ARRVAL(value_ins));
			default: // TODO 其它类型？
				return 0;
		}
	}
	std::size_t value::size() const {
		return length();
	}
	// --------------------------------------------------------------------
	
	TYPE value::typeof() const {
		return TYPE(&value_ins);
	}
	bool value::typeof(const TYPE& t) const {
		zend_uchar t_ = Z_TYPE(value_ins);
		return t == t_ // 类型相同
			|| (t == TYPE::BOOLEAN && (t_ == IS_TRUE || t_ == IS_FALSE))
			|| (t == TYPE::CALLABLE && zend_is_callable(const_cast<zval*>(&value_ins), IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr));
	}
	CLASS value::classof() const {
		assert(typeof(TYPE::OBJECT));
		return CLASS(Z_OBJCE(value_ins));
	}
	bool value::instanceof(const CLASS& c) const {
		return typeof(TYPE::OBJECT) && instanceof_function(Z_OBJCE(value_ins), c);
	}
	// 转换
	// ---------------------------------------------------------------------
	value::operator bool() const {
		return !empty();
	}
	value::operator int() const {
		if(!typeof(TYPE::INTEGER)) {
			throw exception(zend_ce_type_error, "type of '" + TYPE(TYPE::INTEGER).name() + "' is expected, '" + typeof().name() + "' given");
		}
		return Z_LVAL(value_ins);
	}
	value::operator std::int64_t() const {
		if(!typeof(TYPE::INTEGER)) {
			throw exception(zend_ce_type_error, "type of '" + TYPE(TYPE::INTEGER).name() + "' is expected, '" + typeof().name() + "' given");
		}
		return Z_LVAL(value_ins);
	}
	value::operator std::size_t() const {
		if(!typeof(TYPE::INTEGER)) {
			throw exception(zend_ce_type_error, "type of '" + TYPE(TYPE::INTEGER).name() + "' is expected, '" + typeof().name() + "' given");
		}
		return Z_LVAL(value_ins);
	}
	value::operator float() const {
		if(!typeof(TYPE::FLOAT)) {
			throw exception(zend_ce_type_error, "type of '" + TYPE(TYPE::FLOAT).name() + "' is expected, '" + typeof().name() + "' given");
		}
		return Z_DVAL(value_ins);
	}
	value::operator double() const {
		if(!typeof(TYPE::FLOAT)) {
			throw exception(zend_ce_type_error, "type of '" + TYPE(TYPE::FLOAT).name() + "' is expected, '" + typeof().name() + "' given");
		}
		return Z_DVAL(value_ins);
	}
	value::operator std::string() const {
		if(!typeof(TYPE::STRING)) {
			throw exception(zend_ce_type_error, "type of '" + TYPE(TYPE::STRING).name() + "' is expected, '" + typeof().name() + "' given");
		}
		return std::string(Z_STRVAL(value_ins), Z_STRLEN(value_ins));
	}
	value::operator zval*() const {
		return const_cast<zval*>(&value_ins);
	}
	value::operator zend_string*() const {
		assert(typeof(TYPE::STRING));
		return Z_STR(value_ins);
	}
	value::operator zend_object*() const {
		assert(typeof(TYPE::OBJECT));
		return Z_OBJ(value_ins);
	}
	value::operator zend_array*() const {
		assert(typeof(TYPE::ARRAY));
		return Z_ARR(value_ins);
	}
	value::operator zend_class_entry*() const {
		assert(typeof(TYPE::OBJECT));
		return Z_OBJCE(value_ins);
	}
	// (无类型检查)转换
	// ---------------------------------------------------------------------
	bool value::to_boolean() {
		convert_to_boolean(&value_ins);
		return typeof() == TYPE::TRUE;
	}
	std::int64_t value::to_integer(int base) {
		convert_to_long(&value_ins);
		return Z_LVAL(value_ins);
	}
	double value::to_float() {
		convert_to_double(&value_ins);
		return Z_DVAL(value_ins);
	}
	std::string value::to_string() {
		convert_to_string(&value_ins);
		return std::string(Z_STRVAL(value_ins), Z_STRLEN(value_ins));
	}
	// 引用
	// ---------------------------------------------------------------------
	std::uint32_t value::addref() const {
		if(Z_REFCOUNTED(value_ins)) {
			return ++GC_REFCOUNT(Z_COUNTED(value_ins));
		}
		return 1;
	}
	std::uint32_t value::delref() {
		if(Z_REFCOUNTED(value_ins)) {
			return --GC_REFCOUNT(Z_COUNTED(value_ins));
		}
		return 1;
	}
	// 赋值
	// -------------------------------------------------------------------
	value& value::operator =(const value& v) {
		zval_ptr_dtor(&value_ins);
		ZVAL_COPY(&value_ins, &v.value_ins);
		return *this;
	}
	value& value::operator =(value&& v) {
		zval_ptr_dtor(&value_ins);
		ZVAL_COPY_VALUE(&value_ins, &v.value_ins);
		ZVAL_UNDEF(&v.value_ins);
		return *this;
	}
	value& value::operator =(const zval* v) { // 无类型检查
		zval_ptr_dtor(&value_ins);
		ZVAL_COPY(&value_ins, v);
		return *this;
	}
	// 带类型检查
	void value::assign(const value& v, const TYPE& t) {
		if(!v.typeof(t)) throw exception(zend_ce_type_error, "type of '" + t.name() + "' is expected, '" + v.typeof().name() + "' given");
		operator =(static_cast<const value&>(v));
	}
	void value::assign(value&& v, const TYPE& t) {
		if(!v.typeof(t)) throw exception(zend_ce_type_error, "type of '" + t.name() + "' is expected, '" + v.typeof().name() + "' given");
		value::operator =(std::move(v));
	}
	void value::assign(const value& v, const CLASS& c) {
		if(!v.instanceof(c)) throw exception(zend_ce_type_error, "instance of '" + c.name() + "' is expected, '" + v.classof().name() + "' given");
		operator =(static_cast<const value&>(v));
	}
	void value::assign(value&& v, const CLASS& c) {
		if(!v.instanceof(c)) throw exception(zend_ce_type_error, "instance of '" + c.name() + "' is expected, '" + v.classof().name() + "' given");
		value::operator =(std::move(v));
	}
	void value::ptr(void* p) {
		zval_ptr_dtor(&value_ins);
		ZVAL_PTR(&value_ins, p);
	}
	// ---------------------------------------------------------------------
	bool value::operator ==(const value& v) const {
		return Z_PTR(value_ins) == Z_PTR(v.value_ins);
	}
	bool value::operator !=(const value& v) const {
		return Z_PTR(value_ins) != Z_PTR(v.value_ins);
	}
}