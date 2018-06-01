#include "../phpext.h"

namespace php {
	value callable::__invoke(zval* cb, int argc, zval* argv) {
		php::value retval;
		zend_fcall_info fci;
		zend_fcall_info_cache fcc;
		if(FAILURE == zend_fcall_info_init(cb, 0, &fci, &fcc, nullptr, nullptr)) {
			throw php::exception("failed to call user function or callable object");
		}
		fci.params = argv;
		fci.param_count = argc;
		fci.retval = static_cast<zval*>(retval);

		if(zend_call_function(&fci, &fcc) == FAILURE) {
			throw php::exception("failed to call user function or callable object");
		}
		return std::move(retval);
	}
}
