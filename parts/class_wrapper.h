#pragma once

namespace php {
	template <class T>
	struct class_wrapper {
	public:
		T*          cpp;
		zend_object obj;

		static inline T* from_this(zval* val) {
			class_wrapper<T>* wrapper = reinterpret_cast<class_wrapper<T>*>((char*)Z_OBJ_P(val) - XtOffsetOf(class_wrapper<T>, obj));
			return wrapper->cpp;
		}
		static inline T* from_obj(zend_object* obj) {
			class_wrapper<T>* wrapper = reinterpret_cast<class_wrapper<T>*>((char*)obj - XtOffsetOf(class_wrapper<T>, obj));
			return wrapper->cpp;
		}
	};
}
