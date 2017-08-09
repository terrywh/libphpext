#pragma once

namespace php {
	class value;
	class class_base {
	public:
		class_base();
		// !!! prop 更新设置属性必须已经提前“声明”，否则会导致位置错误
		value& prop(const char* name, std::size_t len);
		value& prop(const std::string& name);
		// bool set 为了防止和上面 size_t 对应函数混淆
		value& sprop(const char* name, std::size_t len, value& val);
		value& sprop(const std::string& name, value& val);

		inline void addref() {
			Z_ADDREF(value_);
		}
		inline void delref() {
			Z_DELREF(value_);
		}
	private:
		zval value_;
		friend class value;
		template <class T>
		friend class class_entry;
	};
}
