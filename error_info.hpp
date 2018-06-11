#pragma once

namespace php {
	class error_info {
		// class suppressor {
		// public:
		// 	suppressor(zval* err)
		// 	: error_(err) {
		// 		zval *tmp, rv;
		// 		tmp = zend_read_property(zend_get_exception_base(error_), error_, "previous", sizeof("previous")-1, 1, &rv);
		// 		ZVAL_COPY(&prev_, tmp);
		// 		ZVAL_NULL(&rv);
		// 		zend_update_property(zend_get_exception_base(error_), error_, "previous", sizeof("previous")-1, &rv);
		// 	}
		// 	~suppressor() {
		// 		zend_update_property(zend_get_exception_base(error_), error_, "previous", sizeof("previous")-1, &prev_);
		// 		zval_ptr_dtor(&prev_);
		// 	}
		// private:
		// 	zval* error_;
		// 	zval  prev_;
		// };
	public:
		std::string message;
		std::string file;
		int         line;
		int         code;

		std::string type;
		
		// 用于获取 PHP 全局的错误信息
		error_info()
		: message(PG(last_error_message))
		, file(PG(last_error_file))
		, line(PG(last_error_lineno))
		, code(0) {
			switch (PG(last_error_type)) {
				case E_ERROR:
				case E_CORE_ERROR:
				case E_COMPILE_ERROR:
				case E_USER_ERROR:
				case E_RECOVERABLE_ERROR:
					type = "FATAL";
					break;
				case E_WARNING:
				case E_CORE_WARNING:
				case E_COMPILE_WARNING:
				case E_USER_WARNING:
					type = "WARNING";
					break;
				case E_PARSE:
					type = "ERROR";
					break;
				case E_NOTICE:
				case E_USER_NOTICE:
					type = "NOTICE";
					break;
				case E_STRICT:
					type = "STRICT";
					break;
				case E_DEPRECATED:
				case E_USER_DEPRECATED:
					type = "DEPRECATED";
					break;
				default:
					type = "<UNKNOWN>";
			}
		}
		error_info(const zval* err)
		: type("EXCEPTION") {
			zval rv, *ptr;
			zend_class_entry *ce = Z_OBJCE_P( err );
			// TODO 是否临时关闭 previous 的异常?
			// suppressor(*this); 
			// 按不同类型进行信息提取
			if (ce == zend_ce_parse_error) {
				ptr = zend_read_property(
					zend_get_exception_base(const_cast<zval*>(err)), const_cast<zval*>(err), "message", sizeof("message")-1, 0, &rv);
			}else if(instanceof_function(ce, zend_ce_throwable)) {
				ptr = zend_read_property(
					zend_get_exception_base(const_cast<zval*>(err)), const_cast<zval*>(err), "string", sizeof("string")-1, 1, &rv);
				if(Z_TYPE(rv) != IS_STRING) {
					zend_call_method_with_0_params(const_cast<zval*>(err), ce, nullptr, "__tostring", &rv);
					if (EG(exception)) {
						ZVAL_OBJ(&rv, EG(exception));
						EG(exception) = nullptr;
						zval_ptr_dtor(&rv);

						ZVAL_STRINGL(&rv, "exception in exception::__toString()", 36); 
					}
					assert(Z_TYPE(rv) == IS_STRING);
					zend_update_property(
						zend_get_exception_base(const_cast<zval*>(err)), const_cast<zval*>(err), "string", sizeof("string")-1, &rv);
					zval_ptr_dtor(&rv);
					ptr = zend_read_property(
						zend_get_exception_base(const_cast<zval*>(err)), const_cast<zval*>(err), "string", sizeof("string")-1, 1, &rv);
				}
			}else{ // 非异常类型, 给出类名称
				message.assign("Uncaught exception '");
				message.append(ZSTR_VAL(ce->name), ZSTR_LEN(ce->name));
				message.append("'");
				code = 0;
				file = "<UNKNOWN>";
				line = 0;
				return; // !!!!!!
			}
			message.assign(Z_STRVAL_P(ptr), Z_STRLEN_P(ptr));
			code = zval_get_long(zend_read_property(
				zend_get_exception_base(const_cast<zval*>(err)), const_cast<zval*>(err), "code", sizeof("code")-1, 1, &rv));
			ptr = zend_read_property(
				zend_get_exception_base(const_cast<zval*>(err)), const_cast<zval*>(err), "file", sizeof("file")-1, 1, &rv);
			file.assign(Z_STRVAL_P(ptr), Z_STRLEN_P(ptr));
			line = zval_get_long(zend_read_property(
				zend_get_exception_base(const_cast<zval*>(err)), const_cast<zval*>(err), "line", sizeof("line")-1, 1, &rv));
		}
	};
}