#pragma once

namespace php {
	class value;
	class object;
	class class_base {
	public:
		// !!! prop 更新设置属性必须已经提前“声明”，否则会导致未知错误
		property prop(const char* name, std::size_t len = -1) {
			return php::property(
				reinterpret_cast<php::object&>(value_), 
				php::string(name, (len == -1 ? std::strlen(name) : len)));
		}
		property prop(const php::string& name) {
			return php::property(reinterpret_cast<php::object&>(value_), name);
		}
		operator php::object() {
			return php::object(value_);
		}
	protected:
		zval value_;
		friend class value;
		template <class T>
		friend class class_entry;
	};
}
