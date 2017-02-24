#include "../../core.h"
namespace phpext {
	ini_get::operator std::int64_t() {
		return zend_ini_long(const_cast<char*>(_key.data()), _key.length(), 0);
	}
	ini_get::operator double() {
		return zend_ini_double(const_cast<char*>(_key.data()), _key.length(), 0);
	}
	ini_get::operator string() {
		return string(zend_ini_string(const_cast<char*>(_key.data()), _key.length(), 0));
	}
}