#pragma once

namespace php {
	class value;
	class property;
	class object: public value {
	private:
		static value call(const php::object& obj, const string& name) {
			value rv;
			int r = call_user_function(EG(function_table), obj, name, rv, 0, nullptr);
			assert(r == SUCCESS && "调用方法失败");
			exception::maybe_rethrow();
			return std::move(rv);
		}
		static value call(const php::object& obj, const string& name, const std::vector<value>& argv) {
			zval params[argv.size()];
			for(int i=0;i<argv.size();++i) {
				ZVAL_COPY_VALUE(&params[i], static_cast<zval*>(argv[i]));
			}
			value rv;
			int r = call_user_function(EG(function_table), obj, name, rv, argv.size(), params);
			assert(r == SUCCESS && "调用方法失败");
			exception::maybe_rethrow();
			return std::move(rv);
		}
	public:
		object() {
			object_init(&value_ins);
		}
		object(zend_object* v)
		: value(v) {
			
		}
		object(const CLASS& c) {
			int r = object_init_ex(&value_ins, c);
			assert(r == SUCCESS && "无法创建实例");
			call("__construct");
		}
		object(const CLASS& c, std::vector<value> argv) {
			int r = object_init_ex(&value_ins, c);
			assert(r == SUCCESS && "无法创建实例");
			call("__construct", std::move(argv));
		}
		object(const value& v)
		: value(v, TYPE::OBJECT) {
			
		}
		object(value&& v)
		: value(std::move(v), TYPE::OBJECT) {

		}
		// -----------------------------------------------------------------
		value call(const string& name) {
			return call(*this, name);
		}
		value call(const string& name, const std::vector<value>& argv) {
			return call(*this, name, argv);
		}
		// -----------------------------------------------------------------
		void set(const string& key, const value& val) {
			property::set(*this, key, val);
		}
		value get(const string& key) const {
			return property::get(*this, key);
		}
		property operator [](const char* name) const {
			return property(*this, string(name));
		}
		// ------------------------------------------------------------------
		object& operator =(const value& v) {
			assign(v, TYPE::OBJECT);
			return *this;
		}
		object& operator =(value&& v) {
			assign(std::move(v), TYPE::OBJECT);
			return *this;
		}

		friend class class_base;
	};
}
