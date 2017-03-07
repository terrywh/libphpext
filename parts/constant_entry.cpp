#include "../phpext.h"

namespace php {
	constant_entry::constant_entry(const std::string& name, const value& val)
	:name_(name), value_(val) {

	}
	void constant_entry::fill(zend_constant* entry, int module) {
		ZVAL_COPY(&entry->value, value_.data());
		entry->name  = zend_string_init(name_.c_str(), name_.length(), true);
		entry->flags = CONST_CS | CONST_PERSISTENT;
		entry->module_number = module;
	}
	void constant_entry::declare(zend_class_entry* entry) {
		value_.addref();
		// .... -> _zend_hash_add_or_update_i -> ZVAL_COPY_VALUE()
		zend_declare_class_constant(entry, name_.c_str(), name_.length(), value_.data());
	}
}
