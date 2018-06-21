#include "phpext.h"

namespace php {
	value object::call(zval* obj, const string& name) {
		value rv;
		int r = call_user_function(EG(function_table), obj, name, rv, 0, nullptr);
		assert(r == SUCCESS && "调用方法失败");
		exception::rethrow();
		return std::move(rv);
	}
	value object::call(zval* obj, const string& name, const std::vector<value>& argv) {
		zval params[argv.size()];
		for(int i=0;i<argv.size();++i) {
			ZVAL_COPY_VALUE(&params[i], static_cast<zval*>(argv[i]));
		}
		value rv;
		int r = call_user_function(EG(function_table), obj, name, rv, argv.size(), params);
		assert(r == SUCCESS && "调用方法失败");
		exception::rethrow();
		return std::move(rv);
	}
	object::object() {
		object_init(&val_);
	}
	object::object(std::nullptr_t n)
	: value(n) {

	}
	object::object(class_base* v)
	: value(v) {

	}
	object::object(zval* v, bool ref)
	: value(v, ref) {
		
	}
	object::object(zend_object* v)
	: value(v) {
		
	}
	object::object(const CLASS& c)
	: value(c) {

	}
	object::object(const CLASS& c, std::vector<value> argv)
	: value(c, std::move(argv)) {

	}
	object::object(const value& v)
	: value(v/* , TYPE::OBJECT */) {
		
	}
	object::object(value&& v)
	: value(std::move(v)/* , TYPE::OBJECT */) {

	}
	object::object(const parameter& v)
	: value(v.raw()) {

	}
	object::object(const array_member& v)
	: value(v.raw()) {

	}
	object::object(const property& v)
	: value(v.raw()) {

	}
	// -----------------------------------------------------------------
	value object::call(const string& name) const {
		return call(ptr_, name);
	}
	value object::call(const string& name, const std::vector<value>& argv) const {
		return call(ptr_, name, argv);
	}
	// -----------------------------------------------------------------
	void object::set(const string& key, const value& val) {
		property::set(ptr_, key, val);
	}
	value object::get(const string& key, bool ptr) const {
		zval rv, *op = property::get(ptr_, key, &rv);
		assert(!ptr || op != &rv);
		return value(op, ptr);
	}
	property object::operator [](const char* name) const {
		return property(*this, string(name));
	}
	// ----------------------------------------------------------------
	object& object::operator =(const parameter& v) {
		value::operator =(v.operator value());
		return *this;
	}
	object& object::operator =(const array_member& v) {
		value::operator =(v.operator value());
		return *this;
	}
	object& object::operator =(const property& v) {
		value::operator =(v.operator value());
		return *this;
	}
}
