#pragma once

namespace php {
	class value;
	class parameters;
	class class_base {
	protected:
		zval value_;
	public:
		class_base();
		class_base(const class_base& base);
		class_base(class_base&& base);
		// !!! prop 更新设置属性必须已经提前“声明”，否则会导致位置错误
		value& prop(const char* name, std::size_t len);
		value& prop(const std::string& name);
		inline void _object_set(zend_object* obj) {
			ZVAL_OBJ(&value_, obj);
		}

		friend class value;
	};
}
