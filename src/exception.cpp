#include "phpext.h"

namespace php {
	// 将 PHP 中发生的异常重新抛出到 CPP 中
	void exception::rethrow() {
		if(EG(exception) == nullptr) return;
		exception ex;
		ZVAL_OBJ(&ex.value_ins, EG(exception));
		ex.addref();
		zend_clear_exception();
		throw ex;
	}
	// 将 CPP 中捕获的异常重新抛出到 PHP 中
	void exception::rethrow(const php::exception& ex) {
		zval c;
		ZVAL_COPY(&c, ex);
	 	zend_throw_exception_object(&c);
	}
	exception::exception() {}
	// 注意: 此种构造形式无类型检查
	exception::exception(const zval* v)
	: value(v) {
		
	}
	exception::exception(zend_object* v) {
		if(!instanceof_function(v->ce, zend_ce_throwable)) {
			throw exception(zend_ce_type_error, "instance of '" + CLASS(zend_ce_throwable).name() + "' expected, '" + CLASS(v->ce).name() + "' given");
		}
		ZVAL_OBJ(&value_ins, v);
		addref();
	}
	exception::exception(const CLASS& c, const std::string& message, int code) {
		object e(c, {message, code});
		ZVAL_COPY(&value_ins, e);
	}
	exception::exception(const value& v)
	: value(v, zend_ce_throwable) {

	}
	exception::exception(value&& v)
	: value(std::move(v), zend_ce_throwable) {

	}
	// ----------------------------------------------------------------
	exception& exception::operator =(const value& v) {
		assign(v, zend_ce_throwable);
		return *this;
	}
	exception& exception::operator =(value&& v) {
		assign(std::move(v), zend_ce_throwable);
		return *this;
	}
	// ----------------------------------------------------------------
	error_info exception::info() const {
		return error_info(&value_ins);
	}
	const char* exception::what() const noexcept {
		return info().message.c_str();
	}

}