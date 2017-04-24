#pragma once

namespace php {
	class class_base {
	public:
		class_base();
		class_base(const class_base& base);
		value prop(const char* name);
		// !!! prop 更新设置属性必须已经提前“声明”，否则会导致位置错误
		class_base& prop(const char* name, const value& v);
		inline void _object_set(zend_object* obj) {
			ZVAL_OBJ(&val_, obj);
		}
	private:
		zval val_;
		friend class value;
	};
}
