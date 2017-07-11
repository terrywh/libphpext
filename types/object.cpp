#include "../phpext.h"

namespace php {
	value object::__call(zend_object* obj, const char* name, std::size_t len, int argc, zval argv[], bool silent) {
		value rv, fn(name, len, false);
		zval ov;
		ZVAL_OBJ(&ov, obj);
		if(FAILURE == call_user_function_ex(&obj->ce->function_table, &ov, (zval*)&fn, (zval*)&rv, argc, argv, 1, nullptr) && !silent) {
			zend_error_noreturn(E_USER_ERROR, "failed to call method '%s::%s'", ZSTR_VAL(obj->ce->name), Z_STRVAL_P((zval*)&fn));
		}
		return std::move(rv);
	}
	object object::clone() {
		object o2(Z_OBJCE(value_));
		zend_objects_clone_members(Z_OBJ(o2.value_), Z_OBJ(value_));
		return std::move(o2);
	}
	object object::create(const std::string& name) {
		zend_string* name_ = zend_string_init(name.c_str(), name.length(), false);
		zend_class_entry* ce = zend_lookup_class(name_);
		zend_string_release(name_);
		object obj;
		if(ce != nullptr) {
			ZVAL_OBJ(&obj.value_, zend_objects_new(ce));
			return std::move(obj);
		}else{
			throw exception("failed to create object: class not found");
		}
	}
	object object::create() {
		object obj;
		object_init(&obj.value_);
		return std::move(obj);
	}
	value& object::prop(const char* name, std::size_t len) {
		zval   dv, *rv;
		rv = zend_read_property(Z_OBJCE(value_), &value_, name, len, false, &dv);
		// ZVAL_DEREF(rv);
		return *reinterpret_cast<value*>(rv);
	}
	value& object::sprop(const char* name, std::size_t len, php::value& val) {
		zval   dv, *rv;
		rv = zend_read_property(Z_OBJCE(value_), &value_, name, len, false, &dv);
		// ZVAL_DEREF(rv);
		ZVAL_COPY(rv, static_cast<zval*>(val));
		return *reinterpret_cast<value*>(rv);
	}
	bool object::is_instance_of(const std::string& class_name) const {
		zend_string*      cn = zend_string_init(class_name.c_str(), class_name.length(), false);
		zend_class_entry* ce = zend_lookup_class(cn);
		zend_string_release(cn);
		return instanceof_function(Z_OBJCE(value_), ce);
	}
}
