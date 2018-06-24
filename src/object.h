#pragma once

#include "value.h"
#include "property.h"

namespace php {
	class string;
	class parameter;
	class array_member;	
	class object: public value {
	private:
		static value call(zval* obj, const string& name);
		static value call(zval* obj, const string& name, const std::vector<value>& argv);
	public:
		object(); // undefined
		object(std::nullptr_t n);
		object(class_base* v);
		object(zval* v, bool ref = true);
		object(zend_object* v);
		// use zend_standard_class_def for standard object
		object(const CLASS& c);
		object(const CLASS& c, std::vector<value> argv);
		object(const value& v);
		object(value&& v);
		object(const parameter& v);
		object(const array_member& v);
		object(const property& v);
		// -----------------------------------------------------------------
		value call(const string& name) const;
		value call(const string& name, const std::vector<value>& argv) const;
		// -----------------------------------------------------------------
		void  set(const string& key, const value& val);
		// !!! 虚拟属性
		value get(const string& key, bool ptr = false) const;
		property operator [](const char* name) const;
		// ------------------------------------------------------------------
		using value::operator =;
		object& operator =(const parameter& v);
		object& operator =(const array_member& v);
		object& operator =(const property& v);
		friend class value;
		friend class class_base;
	};
}
