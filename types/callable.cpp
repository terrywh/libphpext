#include "../phpext.h"

namespace php {
	value callable::_invoke(zval* cb, int argc, zval argv[]) {
		value rv;
		if(FAILURE == call_user_function_ex(EG(function_table), nullptr, cb, (zval*)&rv, argc, argv, 1, nullptr)) {
			zend_error(E_USER_ERROR, "failed to invoke callalbe '%s'", zval_get_string(cb));
		}
		return std::move(rv);
	}
	callable::~callable() {
		_zval_dtor(cb_);
	}
	callable::callable(zval* cb):cb_(cb) {
		Z_ADDREF_P(cb_);
	}
	callable::callable(const callable& cb) {
		ZVAL_COPY(cb_, cb.cb_);
	}
	callable::callable(callable&& cb) {
		ZVAL_COPY_VALUE(cb_, cb.cb_);
		cb.cb_ = nullptr;
	}
}
