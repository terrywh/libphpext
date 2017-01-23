#include "property_entry.h"

namespace php {
	property_entry::property_entry(const std::string& name, const value& val, int access)
	:value_(val), access_(access) {
		name_ = zend_string_init(name.c_str(), name.length(), true);
	}
	void property_entry::declare(zend_class_entry* entry) {
		zend_declare_property_ex(entry,
			name_, // name
			value_.data(), access_, /* doc_comment */nullptr);
		value_.addref();
	}
}
