#include "../phpext.h"

namespace php {
	property_entry::property_entry(const std::string& name, std::nullptr_t v, int access)
		: name_(name)
		, value_(v)
		, access_(access) {}
	property_entry::property_entry(const std::string& name, zend_bool v, int access)
		: name_(name)
		, value_(v ? BOOL_YES : BOOL_NO)
		, access_(access) {}
	property_entry::property_entry(const std::string& name, int v, int access)
		: name_(name)
		, value_(v)
		, access_(access) {}
	property_entry::property_entry(const std::string& name, std::int64_t v, int access)
		: name_(name)
		, value_(v)
		, access_(access) {}
	property_entry::property_entry(const std::string& name, double v, int access)
		: name_(name)
		, value_(v)
		, access_(access) {}
	property_entry::property_entry(const std::string& name, const std::string& v, int access)
		: name_(name)
		, value_(zend_string_init(v.c_str(), v.length(), true), true)
		, access_(access) {
			value_.addref();
		}


	property_entry::property_entry(property_entry&& entry)
	: name_(std::move(entry.name_))
	, value_(std::move(entry.value_))
	, access_(entry.access_) {
		entry.access_ = 0;
	}
	void property_entry::declare(zend_class_entry* entry) {
		// ZVAL_COPY_VALUE 故 value_ 内容不能释放
		zend_declare_property(entry, name_.c_str(), name_.length(),
			reinterpret_cast<zval*>(&value_), access_);
	}
}
