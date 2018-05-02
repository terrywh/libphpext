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
	// zend_exceptions.c
	// ZEND_API ZEND_COLD void zend_exception_error(zend_object *ex, int severity)
	char* exception_string(bool previous, size_t* perrlen) {
		if(!EG(exception)) {
			if(perrlen) *perrlen = 0;
			return nullptr;
		};
		static char errstr[4096];
		size_t errlen = 0;
		zval exception, rv;
	
		ZVAL_OBJ(&exception, EG(exception));
		zend_class_entry *ce_exception = Z_OBJCE(exception);
		EG(exception) = nullptr;
		
		if(!previous) {
			ZVAL_NULL(&rv);
			zend_update_property(zend_get_exception_base(&exception), &exception, "previous", sizeof("previous")-1, &rv);
		}
		
		if (ce_exception == zend_ce_parse_error) {
			zend_string* message, *file;
			zend_ulong   line;
			message = zval_get_string(zend_read_property(
				zend_get_exception_base(&exception), &exception, "message", sizeof("message")-1, 0, &rv));
			file = zval_get_string(zend_read_property(
				zend_get_exception_base(&exception), &exception, "file", sizeof("file")-1, 1, &rv));
			line = zval_get_long(zend_read_property(
				zend_get_exception_base(&exception), &exception, "line", sizeof("line")-1, 1, &rv));
			errlen = sprintf(errstr, "[%s:%d] %s\n", ZSTR_VAL(file), line, ZSTR_VAL(message));
			zend_string_release(message);
			zend_string_release(file);
		} else if (instanceof_function(ce_exception, zend_ce_throwable)) {
			zval tmp;
			zend_string* message, *file;
			zend_ulong   line;
			zend_call_method_with_0_params(&exception, ce_exception, nullptr, "__tostring", &tmp);
			if (!EG(exception)) {
				if (Z_TYPE(tmp) != IS_STRING) {
					errlen = sprintf(errstr, "%s::__toString() must return a string\n", ZSTR_VAL(ce_exception->name));
				} else {
					zend_update_property(zend_get_exception_base(&exception), &exception, "string", sizeof("string")-1, &tmp);
				}
			}
			zval_ptr_dtor(&tmp);
			
			if(EG(exception)){
				zval zv;
				ZVAL_OBJ(&zv, EG(exception));
				/* do the best we can to inform about the inner exception */
				if (instanceof_function(ce_exception, zend_ce_exception) || instanceof_function(ce_exception, zend_ce_error)) {
					file = zval_get_string(zend_read_property(zend_get_exception_base(&zv), &zv, "file", sizeof("file")-1, 1, &rv));
					line = zval_get_long(zend_read_property(zend_get_exception_base(&zv), &zv, "line", sizeof("line")-1, 1, &rv));
				}
				errlen = sprintf(errstr, "%s:%d Uncaught %s in exception handling during call to %s::__tostring()\n",
					file && ZSTR_LEN(file) > 0 ? ZSTR_VAL(file) : nullptr, line, ZSTR_VAL(Z_OBJCE(zv)->name), ZSTR_VAL(ce_exception->name));
				if(file) {
					zend_string_release(file);
				}
			}
			message = zval_get_string(zend_read_property(zend_get_exception_base(&exception), &exception, "string", sizeof("string")-1, 1, &rv));
			file = zval_get_string(zend_read_property(zend_get_exception_base(&exception), &exception, "file", sizeof("file")-1, 1, &rv));
			line = zval_get_long(zend_read_property(zend_get_exception_base(&exception), &exception, "line", sizeof("line")-1, 1, &rv));
			
			errlen += sprintf(errstr + errlen, "(%s:%d) Uncaught %s\n",
				file && ZSTR_LEN(file) > 0 ? ZSTR_VAL(file) : nullptr,line, ZSTR_VAL(message));
			zend_string_release(message);
			zend_string_release(file);
		} else {
			errlen = sprintf(errstr, "Uncaught exception '%s'\n", ZSTR_VAL(ce_exception->name));
		}
		zval_ptr_dtor(&exception);
		if(perrlen != nullptr) {
			*perrlen = errlen;
		}
		errstr[errlen] = '\0';
		return errstr;
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

	static void php_simple_ini_parser_cb(zval *arg1, zval *arg2, zval *arg3, int callback_type, zval *arr) {
		switch (callback_type) {

			case ZEND_INI_PARSER_ENTRY:
			if (!arg2) {
				/* bare string - nothing to do */
				break;
			}
			Z_TRY_ADDREF_P(arg2);
			zend_symtable_update(Z_ARRVAL_P(arr), Z_STR_P(arg1), arg2);
			break;

			case ZEND_INI_PARSER_POP_ENTRY:
			{
				zval hash, *find_hash;

				if (!arg2) {
					/* bare string - nothing to do */
					break;
				}

				if (!(Z_STRLEN_P(arg1) > 1 && Z_STRVAL_P(arg1)[0] == '0') && is_numeric_string(Z_STRVAL_P(arg1), Z_STRLEN_P(arg1), NULL, NULL, 0) == IS_LONG) {
					zend_ulong key = (zend_ulong) zend_atol(Z_STRVAL_P(arg1), (int)Z_STRLEN_P(arg1));
					if ((find_hash = zend_hash_index_find(Z_ARRVAL_P(arr), key)) == NULL) {
						array_init(&hash);
						find_hash = zend_hash_index_update(Z_ARRVAL_P(arr), key, &hash);
					}
				} else {
					if ((find_hash = zend_hash_find(Z_ARRVAL_P(arr), Z_STR_P(arg1))) == NULL) {
						array_init(&hash);
						find_hash = zend_hash_update(Z_ARRVAL_P(arr), Z_STR_P(arg1), &hash);
					}
				}

				if (Z_TYPE_P(find_hash) != IS_ARRAY) {
					zval_dtor(find_hash);
					array_init(find_hash);
				}

				if (!arg3 || (Z_TYPE_P(arg3) == IS_STRING && Z_STRLEN_P(arg3) == 0)) {
					Z_TRY_ADDREF_P(arg2);
					add_next_index_zval(find_hash, arg2);
				} else {
					array_set_zval_key(Z_ARRVAL_P(find_hash), arg3, arg2);
				}
			}
			break;

			case ZEND_INI_PARSER_SECTION:
			break;
		}
	}

	static void php_ini_parser_cb_with_sections(zval *arg1, zval *arg2, zval *arg3, int callback_type, zval *arr) {
		if (callback_type == ZEND_INI_PARSER_SECTION) {
			array_init(&BG(active_ini_file_section));
			zend_symtable_update(Z_ARRVAL_P(arr), Z_STR_P(arg1), &BG(active_ini_file_section));
		} else if (arg2) {
			zval *active_arr;

			if (Z_TYPE(BG(active_ini_file_section)) != IS_UNDEF) {
				active_arr = &BG(active_ini_file_section);
			} else {
				active_arr = arr;
			}

			php_simple_ini_parser_cb(arg1, arg2, arg3, callback_type, active_arr);
		}
	}

	php::value parse_ini_file(char* filename) {
		php::value rv;

		zend_file_handle fh;
		memset(&fh, 0x00, sizeof(fh));
		fh.filename = filename;
		fh.type     = ZEND_HANDLE_FILENAME;

		ZVAL_UNDEF(&BG(active_ini_file_section));
		zend_parse_ini_file(&fh, 0, ZEND_INI_SCANNER_TYPED,
				(zend_ini_parser_cb_t) php_ini_parser_cb_with_sections,
				reinterpret_cast<zval*>(&rv));

		return std::move(rv);
	}

}
