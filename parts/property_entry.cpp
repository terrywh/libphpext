#include "../phpext.h"

namespace php {
	property_entry::property_entry(const std::string& name, std::nullptr_t v, int access)
		: name_(zend_string_init(name.c_str(), name.length(), true))
		, value_(v)
		, access_(access) {}
	property_entry::property_entry(const std::string& name, bool v, int access)
		: name_(zend_string_init(name.c_str(), name.length(), true))
		, value_(v)
		, access_(access) {}
	property_entry::property_entry(const std::string& name, int v, int access)
		: name_(zend_string_init(name.c_str(), name.length(), true))
		, value_(v)
		, access_(access) {}
	property_entry::property_entry(const std::string& name, std::int64_t v, int access)
		: name_(zend_string_init(name.c_str(), name.length(), true))
		, value_(v)
		, access_(access) {}
	property_entry::property_entry(const std::string& name, double v, int access)
		: name_(zend_string_init(name.c_str(), name.length(), true))
		, value_(v)
		, access_(access) {}
	property_entry::property_entry(const std::string& name, const std::string& v, int access)
		: name_(zend_string_init(name.c_str(), name.length(), true))
		, value_(v.c_str(), v.length(), true)
		, access_(access) {}


	property_entry::property_entry(property_entry&& entry)
	: name_(entry.name_)
	, value_(std::move(entry.value_))
	, access_(entry.access_) {
		entry.name_ = nullptr;
		entry.value_ = nullptr;
		entry.access_ = 0;
	}
	void property_entry::declare(zend_class_entry* entry) {
		zend_declare_property_ex(entry, name_,
			reinterpret_cast<zval*>(&value_), access_, /* doc_comment */nullptr);
	}
}
