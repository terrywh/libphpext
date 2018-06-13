#include "phpext.h"

namespace php {
	value object::call(const zval* obj, const string& name) {
		value rv;
		int r = call_user_function(EG(function_table), const_cast<zval*>(obj), name, rv, 0, nullptr);
		assert(r == SUCCESS && "调用方法失败");
		exception::rethrow();
		return std::move(rv);
	}
	value object::call(const zval* obj, const string& name, const std::vector<value>& argv) {
		zval params[argv.size()];
		for(int i=0;i<argv.size();++i) {
			ZVAL_COPY_VALUE(&params[i], static_cast<zval*>(argv[i]));
		}
		value rv;
		int r = call_user_function(EG(function_table), const_cast<zval*>(obj), name, rv, argv.size(), params);
		assert(r == SUCCESS && "调用方法失败");
		exception::rethrow();
		return std::move(rv);
	}
	object::object() {
		object_init(&value_ins);
	}
	object::object(class_base* v)
	: value(v) {}
	// 注意: 此种构造形式无类型检查
	object::object(const zval* v)
	: value(v) {
		
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
	: value(v, TYPE::OBJECT) {
		
	}
	object::object(value&& v)
	: value(std::move(v), TYPE::OBJECT) {

	}
	// -----------------------------------------------------------------
	value object::call(const string& name) const {
		return call(&value_ins, name);
	}
	value object::call(const string& name, const std::vector<value>& argv) const {
		return call(&value_ins, name, argv);
	}
	// -----------------------------------------------------------------
	void object::set(const string& key, const value& val) {
		property::set(&value_ins, key, val);
	}
	value object::get(const string& key) const {
		return property(*this, key);
	}
	property object::operator [](const char* name) const {
		return property(*this, string(name));
	}
	// ------------------------------------------------------------------
	object& object::operator =(const value& v) {
		assign(v, TYPE::OBJECT);
		return *this;
	}
	object& object::operator =(value&& v) {
		assign(std::move(v), TYPE::OBJECT);
		return *this;
	}
	object& object::operator =(const zval* v) {
		value::operator =(v);
		return *this;
	}
}
