#include "../vendor.h"

namespace php {
	void fail(const std::string& msg) {
		zend_error(E_USER_ERROR, msg.c_str());
	}
	void warn(const std::string& msg) {
		zend_error(E_USER_WARNING, msg.c_str());
	}
	void info(const std::string& msg) {
		zend_error(E_USER_NOTICE, msg.c_str());
	}
}
