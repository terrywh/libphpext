#include "../phpext.h"

namespace php {
	constant_entry::constant_entry(const std::string& name, const php::value& v)
		:name_(name), value_(v) {}

	void constant_entry::declare(int module) {
		zend_constant entry;
		ZVAL_COPY(&entry.value, reinterpret_cast<zval*>(&value_));
		entry.name  = zend_string_init(name_.c_str(), name_.length(), true);
		entry.flags = CONST_CS | CONST_PERSISTENT;
		entry.module_number = module;
		zend_register_constant(&entry);
	}
	void constant_entry::declare(zend_class_entry* entry) {
		zend_declare_class_constant(entry, name_.c_str(), name_.length(), reinterpret_cast<zval*>(&value_));
	}
}
