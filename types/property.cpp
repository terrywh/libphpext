#include "../phpext.h"

namespace php {
	property::property(const php::object& obj, const php::string& name, bool silent)
	: object_(obj), name_(name) {
		zval  rv, *vv;
		vv = zend_read_property(object_, object_, name_.c_str(), name_.length(), false, &rv);
		ZVAL_COPY(&value_, vv);
	}
	property& property::operator =(const php::value& val) {
		value::operator=(val);
		zend_update_property(object_, object_, name_.c_str(), name_.length(), &value_);
	}
	property& property::operator =(php::value&& val) {
		value::operator=(std::move(val));
		zend_update_property(object_, object_, name_.c_str(), name_.length(), &value_);
	}
}
