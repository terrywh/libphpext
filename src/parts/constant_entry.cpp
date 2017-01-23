#include "constant_entry.h"

namespace php {
	constant_entry::constant_entry(const std::string& name, const value& val)
	:name_(name), value_(val) {

	}
	void constant_entry::fill(zend_constant* entry) {
		ZVAL_COPY(&entry->value, value_.data());
		entry->name  = zend_string_init(name_.c_str(), name_.length(), true);
		entry->flags = CONST_CS | CONST_PERSISTENT;
		// entry->module_number   = module;
	}
}
