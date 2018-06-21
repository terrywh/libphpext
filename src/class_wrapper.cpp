#include "phpext.h"

namespace php {
	class_base* native(zend_object* obj) {
		class_wrapper* wrapper = reinterpret_cast<class_wrapper*>((char*)obj - XtOffsetOf(class_wrapper, obj));
		return wrapper->cpp.get();
	}
}