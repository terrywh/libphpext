#include "../phpext.h"

namespace php {
	value value::callable(const char* str) {
		return callable(str, std::strlen(str));
	}
	value value::callable(const std::string& str) {
		return callable(str.c_str(), str.length());
	}
	value value::callable(const char* str, std::size_t len) {
		value c;
		ZVAL_NEW_STR(c.val_, zend_string_init(str, len, false));
		zend_make_callable(c.val_, nullptr);
		return std::move(c);
	}

	value value::operator() ()
	{
		return invoke_(0, nullptr);
	}
	value value::invoke_(int argc, zval* argv)
	{
		if(!is_callable()) throw exception("type error: callable expected");
		zend_fcall_info_cache fcc_;// = empty_fcall_info_cache;
		zend_fcall_info       fci_;
		zend_fcall_info_init(val_, 0, &fci_, &fcc_, nullptr, nullptr);
		value rv(nullptr);
		// zend_fcall_info_argp(&fci_, argc, argv);
		fci_.param_count = argc;
		fci_.params = argv;
		if(zend_fcall_info_call(&fci_, &fcc_, rv.data(), nullptr) == FAILURE) {
			throw exception("failed to invoke callable", exception::INVOKE_CALLABLE_FAILED);
		}
		// zend_fcall_info_args_clear(&fci_, 1);
		return std::move(rv);
	}
}
