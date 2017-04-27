#include "../phpext.h"

namespace php {
	constant_entry::constant_entry(const std::string& name, std::nullptr_t v)
		:name_(name), value_(v) {}
	constant_entry::constant_entry(const std::string& name, bool v)
	 	:name_(name), value_(v) {}
	constant_entry::constant_entry(const std::string& name, int v)
	 	:name_(name), value_(v) {}
	constant_entry::constant_entry(const std::string& name, std::int64_t v)
	 	:name_(name), value_(v) {}
	constant_entry::constant_entry(const std::string& name, double v)
	 	:name_(name), value_(v) {}
	constant_entry::constant_entry(const std::string& name, const std::string& v)
	 	:name_(name), value_(v.c_str(), v.length(), true) {}

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
