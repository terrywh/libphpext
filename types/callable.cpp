#include "../phpext.h"

namespace php {
	value callable::__invoke(zval* cb, int argc, zval* argv, bool silent) {
		value rv;
		// 简化直接调用 __closure::fn_ 似乎会导致某种内存问题, 原因不明...
		if(FAILURE == call_user_function_ex(EG(function_table), nullptr, cb, (zval*)&rv, argc, argv, 1, nullptr) && !silent) {
			zend_error_noreturn(E_USER_ERROR, "failed to invoke callable '%s'", zval_get_string(cb));
		}
		return std::move(rv);
	}
}
