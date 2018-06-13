#include "phpext.h"

namespace php {
	value class_base::get(const php::string& key) const {
		return php::property(&obj_, key);
	}
	void class_base::set(const php::string& key, const php::value& val) const {
		property::set(&obj_, key, val);
	}
	php::value class_base::call(const php::string& name) const {
		return object::call(&obj_, name);
	}
	php::value class_base::call(const php::string& name, const std::vector<php::value>& argv) const {
		return object::call(&obj_, name, argv);
	}
	property class_base::operator [](const php::string& name) const {
		return php::property(&obj_, name);
	}
	property class_base::property(const php::string& name) const {
		return php::property(&obj_, name);
	}
}
