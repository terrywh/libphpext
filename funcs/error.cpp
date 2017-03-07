#include "../vendor.h"

namespace php {
	void fail(const char *format, va_list argv) {
		zend_error(E_USER_ERROR, format, argv);
	}
	void warn(const char *format, va_list argv) {
		zend_error(E_USER_WARNING, format, argv);
	};
	void info(const char *format, va_list argv) {
		zend_error(E_USER_NOTICE, format, argv);
	};

	void fail(const char *format) {
		zend_error(E_USER_ERROR, format);
	}
	void warn(const char *format) {
		zend_error(E_USER_WARNING, format);
	};
	void info(const char *format) {
		zend_error(E_USER_NOTICE, format);
	};
}