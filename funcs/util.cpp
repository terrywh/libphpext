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
	const char * error_type_name(int type) {
		switch (type) {
			case E_ERROR:
			case E_CORE_ERROR:
			case E_COMPILE_ERROR:
			case E_USER_ERROR:
			case E_RECOVERABLE_ERROR:
				return "FATAL";
			case E_WARNING:
			case E_CORE_WARNING:
			case E_COMPILE_WARNING:
			case E_USER_WARNING:
				return "WARNING";
			case E_PARSE:
				return "ERROR";
			case E_NOTICE:
			case E_USER_NOTICE:
				return "NOTICE";
			case E_STRICT:
				return "STRICT";
			case E_DEPRECATED:
			case E_USER_DEPRECATED:
				return "DEPRECATED";
			default:
				return "UNKNOWN";
		}
		
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
	static void user_shutdown_function_dtor(zval *zv) {
		int i;
		php_shutdown_function_entry *shutdown_function_entry = (php_shutdown_function_entry*)Z_PTR_P(zv);

		for (i = 0; i < shutdown_function_entry->arg_count; i++) {
			zval_ptr_dtor(&shutdown_function_entry->arguments[i]);
		}
		efree(shutdown_function_entry->arguments);
		efree(shutdown_function_entry);
	}
	void register_shutdown_function(const php::callable& cb) {
		if (!BG(user_shutdown_function_names)) {
			ALLOC_HASHTABLE(BG(user_shutdown_function_names));
			zend_hash_init(BG(user_shutdown_function_names), 0, NULL, user_shutdown_function_dtor, 0);
		}
		php_shutdown_function_entry shutdown_function_entry;
		shutdown_function_entry.arg_count = 1;
		shutdown_function_entry.arguments = (zval *) safe_emalloc(sizeof(zval), shutdown_function_entry.arg_count, 0);
		ZVAL_COPY(&shutdown_function_entry.arguments[0], static_cast<zval*>(cb));
		// for (i = 0; i < shutdown_function_entry.arg_count; i++) {
		// 	if (Z_REFCOUNTED(shutdown_function_entry.arguments[i])) Z_ADDREF(shutdown_function_entry.arguments[i]);
		// }
		zend_hash_next_index_insert_mem(BG(user_shutdown_function_names), &shutdown_function_entry, sizeof(php_shutdown_function_entry));
	}
}
