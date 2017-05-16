#include "../phpext.h"

namespace php {
	void fail(const std::string& msg) {
		zend_error(E_USER_ERROR, msg.c_str());
	}
	void fail(const char *format, ...) {
		va_list args;

		va_start(args, format);
		php_verror(NULL, "", E_USER_ERROR, format, args);
		va_end(args);
	}

	void warn(const std::string& msg) {
		zend_error(E_USER_WARNING, msg.c_str());
	}
	void warn(const char *format, ...) {
		va_list args;

		va_start(args, format);
		php_verror(NULL, "", E_USER_WARNING, format, args);
		va_end(args);
	}


	void info(const std::string& msg) {
		zend_error(E_USER_NOTICE, msg.c_str());
	}
	void info(const char *format, ...) {
		va_list args;

		va_start(args, format);
		php_verror(NULL, "", E_USER_NOTICE, format, args);
		va_end(args);
	}

	const char* ini_get(char* name, uint name_len) {
		return zend_ini_string(name, name_len, 0);
	}

	php::string strtoupper(char* str, size_t len) {
		php::string s(str, len);
		php_strtoupper(s.data(), len);
		return std::move(s);
	}
	php::string strtolower(char* str, size_t len) {
		php::string s(str, len);
		php_strtolower(s.data(), len);
		return std::move(s);
	}

	php::value make_exception(const std::string& message, int code) {
		php::object ex = php::object::create("Exception");
		ex.call("__construct", message, code);
		return std::move(ex);
	}

}
