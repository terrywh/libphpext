#include "../phpext.h"

namespace php {
	property_entry::property_entry(const php::string& name, const php::value& v, int access)
		: key_(name)
		, val_(v)
		, access_(access) {}

	property_entry::property_entry(property_entry&& entry)
	: key_(std::move(entry.key_))
	, val_(std::move(entry.val_))
	, access_(entry.access_) {
		entry.access_ = 0;
	}
	void property_entry::declare(zend_class_entry* entry) {
		// 属性声明要求使用 persistent , 这里需要重新创建
		switch(val_.type()) {
		case IS_NULL:
			zend_declare_property_null(entry, key_.c_str(), key_.length(),
				access_);
			break;
		case IS_TRUE:
		case IS_FALSE:
			zend_declare_property_bool(entry, key_.c_str(), key_.length(),
				val_.to_bool(), access_);
			break;
		case IS_LONG:
			zend_declare_property_long(entry, key_.c_str(), key_.length(),
				val_.to_long(), access_);
			break;
		case IS_DOUBLE:
			zend_declare_property_double(entry, key_.c_str(), key_.length(),
				val_.to_double(), access_);
			break;
		case IS_STRING:
			zend_declare_property_stringl(entry, key_.c_str(), key_.length(),
				static_cast<php::string&>(val_).c_str(), static_cast<php::string&>(val_).length(),
				access_);
			break;
		default: {
			std::string msg;
			msg.append("property '");
			msg.append(key_.c_str(), key_.length());
			msg.append("' can not be delared: illegal type");
			throw std::runtime_error(msg);
		}}
	}
}
