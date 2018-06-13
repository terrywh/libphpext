#pragma once

namespace php {
	class class_wrapper;
	class object: public value {
	private:
		static value call(const zval* obj, const string& name);
		static value call(const zval* obj, const string& name, const std::vector<value>& argv);
	public:
		object();
		object(class_base* v);
		// 注意: 此种构造形式无类型检查
		object(const zval* v);
		object(zend_object* v);
		object(const CLASS& c);
		object(const CLASS& c, std::vector<value> argv);
		object(const value& v);
		object(value&& v);
		// -----------------------------------------------------------------
		value call(const string& name) const;
		value call(const string& name, const std::vector<value>& argv) const;
		// -----------------------------------------------------------------
		void set(const string& key, const value& val);
		value get(const string& key) const;
		property operator [](const char* name) const;
		// ------------------------------------------------------------------
		object& operator =(const value& v);
		object& operator =(value&& v);
		object& operator =(const zval* v);
		friend class value;
		friend class class_base;
	};
}
