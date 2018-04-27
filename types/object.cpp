#include "../phpext.h"

namespace php {
	php::value object::__call(zval* obj, zval* fn, int argc, zval* argv, bool silent) {
		php::value rv;
		if(FAILURE == call_user_function_ex(&Z_OBJCE_P(obj)->function_table, obj, fn, (zval*)&rv, argc, argv, 1, nullptr) && !silent) {
			zend_error_noreturn(E_USER_ERROR, "failed to call method '%s::%s'", ZSTR_VAL(Z_OBJCE_P(obj)->name), Z_STRVAL_P(fn));
		}
		return std::move(rv);
	}
	object object::create(const std::string& name) {
		zend_string*   name_ = zend_string_init(name.c_str(), name.length(), false);
		zend_class_entry* ce = zend_lookup_class(name_);
		zend_string_release(name_);
		if(ce != nullptr) {
			return create(ce);
		}else{
			throw exception("failed to create object: class not found");
		}
	}
	object object::create(zend_class_entry* ce) {
		object obj;
		if(ce->create_object) {
			ZVAL_OBJ(&obj.value_, ce->create_object(ce));
		}else{
			ZVAL_OBJ(&obj.value_, zend_objects_new(ce));
		}
		return std::move(obj);
	}
	object object::create_exception(const std::string& message, int code) {
		object obj;
		ZVAL_OBJ(&obj.value_, zend_ce_exception->create_object(zend_ce_exception));
		obj.call("__construct", {message, code});
		return std::move(obj);
	}
	object object::create() {
		object obj;
		object_init(&obj.value_);
		return std::move(obj);
	}
	// value& object::sprop(const char* name, std::size_t len, php::value& val) {
	// 	zval* rv;
	// 	rv = zend_read_static_property(Z_OBJCE(value_), name, len, false);
	// 	return *reinterpret_cast<value*>(rv);
	// }
	bool object::is_instance_of(const std::string& class_name) const {
		zend_string*      cn = zend_string_init(class_name.c_str(), class_name.length(), false);
		zend_class_entry* ce = zend_lookup_class(cn);
		zend_string_release(cn);
		return instanceof_function(Z_OBJCE(value_), ce);
	}
	bool object::is_instance_of(zend_class_entry* ce) const {
		return instanceof_function(Z_OBJCE(value_), ce);
	}
}
