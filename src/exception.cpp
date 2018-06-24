#include "vendor.h"
#include "exception.h"

#include "object.h"

namespace php {
	// 将 PHP 中发生的异常重新抛出到 CPP 中
	void exception::rethrow() {
		if(EG(exception) == nullptr) return;
		exception ex;
		ZVAL_OBJ(&ex.val_, EG(exception));
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
	exception::exception(std::nullptr_t n)
	: value(n) {
		
	}
	exception::exception(zval* v, bool ref)
	: value(v, ref) {
		
	}
	exception::exception(zend_object* v) {
		assert(instanceof_function(v->ce, zend_ce_throwable));
		ZVAL_OBJ(&val_, v);
		addref();
	}
	exception::exception(const CLASS& c, const std::string& message, int code)
	: value(c, {message, code}) {
		
	}
	exception::exception(const value& v)
	: value(v/* , CLASS(zend_ce_throwable) */) {

	}
	exception::exception(value&& v)
	: value(std::move(v)/* , CLASS(zend_ce_throwable) */) {

	}
	// ----------------------------------------------------------------
	error_info exception::info() const {
		return error_info(ptr_);
	}
	const char* exception::what() const noexcept {
		return info().message.c_str();
	}

}