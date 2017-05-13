#include "../phpext.h"

namespace php {
	class_base::class_base() {
		ZVAL_UNDEF(&value_);
	}
	value& class_base::prop(const char* name, std::size_t len) {
		object& obj = *reinterpret_cast<object*>(&value_);
		return obj.prop(name, len);
	}
	value& class_base::prop(const std::string& name) {
		object& obj = *reinterpret_cast<object*>(&value_);
		return obj.prop(name.c_str(), name.length());
	}
	value& class_base::prop(const std::string& name, value& val, bool set) {
		object& obj = *reinterpret_cast<object*>(&value_);
		return obj.prop(name, val, set);
	}
}
