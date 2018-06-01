#include "../phpext.h"

namespace php {
	exception::exception_previous::exception_previous(exception& ex)
	: ex_(ex) {
		zval *tmp, rv;
		tmp = zend_read_property(zend_get_exception_base(&ex_.value_), &ex_.value_, "previous", sizeof("previous")-1, 1, &rv);
		ZVAL_COPY(&prev_, tmp);
		ZVAL_NULL(&rv);
		zend_update_property(zend_get_exception_base(&ex_.value_), &ex_.value_, "previous", sizeof("previous")-1, &rv);
	}
	exception::exception_previous::~exception_previous() {
		zend_update_property(zend_get_exception_base(&ex_.value_), &ex_.value_, "previous", sizeof("previous")-1, &prev_);
	}
	exception::exception(const php::string& message, int code) {
		ZVAL_OBJ(&value_, zend_ce_exception->create_object(zend_ce_exception));
		reinterpret_cast<php::object&>(value_).call("__construct", 11, {message, code});
	}
	bool exception::has() {
		return EG(exception) != nullptr;
	}
	exception exception::get() {
		assert(EG(exception) != nullptr);
		exception ex;
		ZVAL_OBJ(&ex.value_, EG(exception));
		ex.addref();
		return ex;
	}
	void exception::off() {
		assert(EG(exception) != nullptr);
		zend_clear_exception();
	}
	bool exception::global() {
		return Z_OBJ(value_) == EG(exception);
	}
	// 下面流程主要参考 zend_exception.c 中
	// ZEND_API ZEND_COLD void zend_exception_error(zend_object *ex, int severity)
	// 函数的相关代码
	php::string exception::message() const noexcept {
		exception_previous(const_cast<exception&>(*this)); // 临时将 previous 属性清除, 完成后恢复
		zval rv;
		zend_class_entry *ce = Z_OBJCE(value_);
		if (ce == zend_ce_parse_error) {
			return zend_read_property(
				zend_get_exception_base(const_cast<zval*>(&value_)), const_cast<zval*>(&value_), "message", sizeof("message")-1, 0, &rv);
		}else if(instanceof_function(ce, zend_ce_throwable)) {
			zval* str = zend_read_property(zend_get_exception_base(const_cast<zval*>(&value_)), const_cast<zval*>(&value_), "string", sizeof("string")-1, 1, &rv);
			if(Z_TYPE_P(str) != IS_STRING) {
				zend_call_method_with_0_params(const_cast<zval*>(&value_), ce, nullptr, "__tostring", &rv);
				if (!EG(exception)) {
					if(Z_TYPE(rv) == IS_STRING) {
						goto STRING_SUCCESS;
					}
				}
				ZVAL_STRINGL(&rv, "", 0); // 默认空字符串
STRING_SUCCESS:
				php::string str(rv);
				zend_update_property(zend_get_exception_base(const_cast<zval*>(&value_)), const_cast<zval*>(&value_), "string", sizeof("string")-1, &rv);
				zval_ptr_dtor(&rv);
				if(EG(exception)) { // 这里不处理间接引发的异常
					ZVAL_OBJ(&rv, EG(exception));
					EG(exception) = nullptr;
					zval_ptr_dtor(&rv);
				}
				return std::move(str);
			}else{
				return php::string(str);
			}
		}else{
			char   str[256];
			size_t len = sprintf(str, "Uncaught exception '%.*s'", ZSTR_LEN(ce->name), ZSTR_VAL(ce->name));
			return php::string(str, len);
		}
	}
	php::string exception::file() const noexcept {
		zend_class_entry *ce = Z_OBJCE(value_);
		if(ce == zend_ce_parse_error || instanceof_function(ce, zend_ce_throwable)) {
			zval rv;
			return zend_read_property(zend_get_exception_base(const_cast<zval*>(&value_)), const_cast<zval*>(&value_), "file", sizeof("file")-1, 1, &rv);
		}else{
			return php::string(0);
		}
	}
	int exception::line() const noexcept {
		zend_class_entry *ce = Z_OBJCE(value_);
		if(ce == zend_ce_parse_error || instanceof_function(ce, zend_ce_throwable)) {
			zval rv;
			return zval_get_long(zend_read_property(zend_get_exception_base(const_cast<zval*>(&value_)), const_cast<zval*>(&value_), "line", sizeof("line")-1, 1, &rv));
		}else{
			return 0;
		}
	}
	int exception::code() const noexcept {
		zend_class_entry *ce = Z_OBJCE(value_);
		if(instanceof_function(ce, zend_ce_throwable)) {
			zval rv;
			return zval_get_long(zend_read_property(zend_get_exception_base(const_cast<zval*>(&value_)), const_cast<zval*>(&value_), "code", sizeof("code")-1, 1, &rv));
		}else{
			return 0;
		}
	}
}
