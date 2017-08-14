#include "../phpext.h"

namespace php {
	value& class_base::prop(const char* name, std::size_t len) {
		object& obj = *reinterpret_cast<object*>(&value_);
		return obj.prop(name, len);
	}
	value& class_base::prop(const std::string& name) {
		object& obj = *reinterpret_cast<object*>(&value_);
		return obj.prop(name.c_str(), name.length());
	}
	value& class_base::sprop(const char* name, std::size_t len, value& val) {
		object& obj = *reinterpret_cast<object*>(&value_);
		return obj.sprop(name, len, val);
	}
	value& class_base::sprop(const std::string& name, value& val) {
		object& obj = *reinterpret_cast<object*>(&value_);
		return obj.sprop(name, val);
	}
}
