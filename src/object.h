#pragma once

namespace php {
	class class_wrapper;
	class object: public value {
	private:
		static value call(zval* obj, const string& name);
		static value call(zval* obj, const string& name, const std::vector<value>& argv);
	public:
		object();
		object(class_base* v);
		object(zval* v, bool ref = true);
		object(zend_object* v);
		object(const CLASS& c);
		object(const CLASS& c, std::vector<value> argv);
		object(const value& v);
		object(value&& v);
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
		friend class value;
		friend class class_base;
	};
}
