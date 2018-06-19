#pragma once

namespace php {
	struct class_wrapper {
		std::unique_ptr<class_base> cpp;
		zend_object obj;
	};
	template <class CLASS_TYPE>
	CLASS_TYPE* native(zend_object* obj) {
		class_wrapper* wrapper = reinterpret_cast<class_wrapper*>((char*)obj - XtOffsetOf(class_wrapper, obj));
		return static_cast<CLASS_TYPE*>(wrapper->cpp.get());
	}
	// template <class CLASS_TYPE>
	// CLASS_TYPE* native(zval* obj) {
	// 	return native<CLASS_TYPE>(Z_OBJ_P(obj));
	// }
}