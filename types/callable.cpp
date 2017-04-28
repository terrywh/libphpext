#include "../phpext.h"

namespace php {
	value callable::__invoke(zval* cb, int argc, zval argv[], bool silent) {
		value rv;
		if(FAILURE == call_user_function_ex(EG(function_table), nullptr, cb, (zval*)&rv, argc, argv, 1, nullptr) && !silent) {
			zend_error_noreturn(E_USER_ERROR, "failed to invoke callalbe '%s'", zval_get_string(cb));
		}
		return std::move(rv);
	}
	callable::~callable() {
		_zval_dtor(&cb_);
	}
	callable::callable(zval* cb) {
		ZVAL_COPY(&cb_, cb);
	}
	callable::callable(const callable& cb) {
		ZVAL_COPY(&cb_, &cb.cb_);
	}
	callable::callable(callable&& cb) {
		ZVAL_COPY_VALUE(&cb_, &cb.cb_);
		ZVAL_UNDEF(&cb.cb_);
	}
}
