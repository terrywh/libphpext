#include "../phpext.h"

namespace php {
	php::value object::__call(zval* obj, zval* fn, int argc, zval* argv) {
		php::value rv;
		if(FAILURE == call_user_function_ex(&Z_OBJCE_P(obj)->function_table, obj, fn, (zval*)&rv, argc, argv, 1, nullptr)) {
			throw php::exception("failed to call object method");
		}
		// if(exception::has()) {
		// 	throw *exception::get();
		// }
		return std::move(rv);
	}
	object object::create(const php::string& name) {
		zend_class_entry* ce = zend_lookup_class(name);
		if(ce != nullptr) {
			return create(ce);
		}else{
			throw exception("failed to create object: class not found");
		}
	}
	object object::create(zend_class_entry* ce) {
		object obj;
		_object_init_ex(&obj.value_, ce);
		return std::move(obj);
	}
	object object::create() {
		object obj;
		object_init(&obj.value_);
		return std::move(obj);
	}
	property object::prop(const php::string& name) const {
		return php::property(*this, name);
	}
	property object::prop(const char* name, std::size_t size) const {
		return php::property(*this, name, (size == -1 ? std::strlen(name) : size));
	}
	bool object::is_instance_of(const php::string& class_name) const {
		zend_class_entry* ce = zend_lookup_class(class_name);
		return instanceof_function(Z_OBJCE(value_), ce);
	}
	bool object::is_instance_of(zend_class_entry* ce) const {
		return instanceof_function(Z_OBJCE(value_), ce);
	}
}
