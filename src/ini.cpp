#include "vendor.h"
#include "ini.h"

namespace php {
	ini::ini(const std::string& key)
	: key_(key) {

	}
	const char* ini::c_str() {
		return data();
	}
	char* ini::data() {
		return zend_ini_string(const_cast<char*>(key_.data()), key_.length(), 0);
	}
	ini::operator std::int64_t() {
		return zend_ini_long(const_cast<char*>(key_.data()), key_.length(), 0);
	}
	ini::operator std::size_t() {
		return zend_ini_long(const_cast<char*>(key_.data()), key_.length(), 0);
	}
	ini::operator std::string() {
		return std::string(data());
	}
	ini::operator double() {
		return zend_ini_double(const_cast<char*>(key_.data()), key_.length(), 0);
	}
}
