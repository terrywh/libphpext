#pragma once

namespace php {
	class value;
	class object;
	class class_base {
	public:
		// !!! prop 更新设置属性必须已经提前“声明”，否则会导致未知错误
		property prop(const char* name, std::size_t len);
		property prop(const std::string& name);
		// bool set 为了防止和上面 size_t 对应函数混淆
		// value& sprop(const char* name, std::size_t len, value& val);
		// value& sprop(const std::string& name, value& val);

		inline class_base* addref() {
			Z_ADDREF(value_);
		}
		inline class_base* delref() {
			Z_DELREF(value_);
		}
		inline php::object& obj() {
			return reinterpret_cast<php::object&>(value_);
		}
	private:
		zval value_;
		friend class value;
		template <class T>
		friend class class_entry;
	};
}
