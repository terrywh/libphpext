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
	// 参考 php/main/main.c  php_error_cb 相关代码
	static const char * error_type(int type) {
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
	char* error_string(size_t* perrlen) {
		static char errstr[8192];
		size_t errlen = 0;
		if (PG(last_error_message)) {
			errlen = sprintf(errstr, "(%s) %s:%d %s\n",
				error_type(PG(last_error_type)),
				PG(last_error_file), PG(last_error_lineno), 
				PG(last_error_message));
			if(perrlen != nullptr) {
				*perrlen = errlen;
			}
			return errstr;
		}else{
			return nullptr;
		}
	}
	// zend_exceptions.c
	// ZEND_API ZEND_COLD void zend_exception_error(zend_object *ex, int severity)
	char* exception_string(size_t* perrlen) {
		if(!EG(exception)) {
			if(perrlen) *perrlen = 0;
			return nullptr;
		};
		static char errstr[8192];
		size_t errlen = 0;
		zval exception, rv;
	
		ZVAL_OBJ(&exception, EG(exception));
		zend_class_entry *ce_exception = Z_OBJCE(exception);
		EG(exception) = nullptr;
		
		// if(!previous) {
			ZVAL_NULL(&rv);
			zend_update_property(zend_get_exception_base(&exception), &exception, "previous", sizeof("previous")-1, &rv);
		// }
		
		if (ce_exception == zend_ce_parse_error) {
			zend_string* message, *file;
			zend_ulong   line;
			message = zval_get_string(zend_read_property(
				zend_get_exception_base(&exception), &exception, "message", sizeof("message")-1, 0, &rv));
			file = zval_get_string(zend_read_property(
				zend_get_exception_base(&exception), &exception, "file", sizeof("file")-1, 1, &rv));
			line = zval_get_long(zend_read_property(
				zend_get_exception_base(&exception), &exception, "line", sizeof("line")-1, 1, &rv));
			errlen = sprintf(errstr, "%s:%d %s\n", ZSTR_VAL(file), line, ZSTR_VAL(message));
			zend_string_release(message);
			zend_string_release(file);
		} else if (instanceof_function(ce_exception, zend_ce_throwable)) {
			zval tmp;
			zend_string* message, *file;
			zend_ulong   line;
			zend_call_method_with_0_params(&exception, ce_exception, nullptr, "__tostring", &tmp);
			if (!EG(exception)) {
				// if (Z_TYPE(tmp) != IS_STRING) {
					// errlen = sprintf(errstr, "%s::__toString() must return a string\n", ZSTR_VAL(ce_exception->name));
				// } else {
				if(Z_TYPE(tmp) == IS_STRING) {
					zend_update_property(zend_get_exception_base(&exception), &exception, "string", sizeof("string")-1, &tmp);
				}
			}
			zval_ptr_dtor(&tmp);
			
			if(EG(exception)){
				zval ex;
				ZVAL_OBJ(&ex, EG(exception));
				EG(exception) = nullptr;
				zval_ptr_dtor(&ex);
			// 	/* do the best we can to inform about the inner exception */
			// 	if (instanceof_function(ce_exception, zend_ce_exception) || instanceof_function(ce_exception, zend_ce_error)) {
			// 		file = zval_get_string(zend_read_property(zend_get_exception_base(&zv), &zv, "file", sizeof("file")-1, 1, &rv));
			// 		line = zval_get_long(zend_read_property(zend_get_exception_base(&zv), &zv, "line", sizeof("line")-1, 1, &rv));
			// 	}
			// 	errlen = sprintf(errstr, "%s:%d Uncaught %s in exception handling during call to %s::__tostring()\n",
			// 		file && ZSTR_LEN(file) > 0 ? ZSTR_VAL(file) : nullptr, line, ZSTR_VAL(Z_OBJCE(zv)->name), ZSTR_VAL(ce_exception->name));
			// 	if(file) {
			// 		zend_string_release(file);
			// 	}
			}
			message = zval_get_string(zend_read_property(zend_get_exception_base(&exception), &exception, "string", sizeof("string")-1, 1, &rv));
			file = zval_get_string(zend_read_property(zend_get_exception_base(&exception), &exception, "file", sizeof("file")-1, 1, &rv));
			line = zval_get_long(zend_read_property(zend_get_exception_base(&exception), &exception, "line", sizeof("line")-1, 1, &rv));
			
			// errlen += sprintf(errstr + errlen, "%s:%d Uncaught %s\n",
			errlen = sprintf(errstr, "(EXCEPTION) %s:%d Uncaught %s\n",
				file && ZSTR_LEN(file) > 0 ? ZSTR_VAL(file) : nullptr,line, ZSTR_VAL(message));
			zend_string_release(message);
			zend_string_release(file);
		} else {
			errlen = sprintf(errstr, "(EXCEPTION) Uncaught exception '%s'\n", ZSTR_VAL(ce_exception->name));
		}
		// 未遵循标准流程, 此处一般仅为在日志文件中额外输出异常记录
		// zval_ptr_dtor(&exception);
		EG(exception) = Z_OBJ(exception); // 还原
		if(perrlen != nullptr) {
			*perrlen = errlen;
		}
		errstr[errlen] = '\0';
		return errstr;
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
