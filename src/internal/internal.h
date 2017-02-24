#ifdef __GNUC__
#define VARIABLE_IS_NOT_USED __attribute__ ((unused))
#endif

#define PHP_CORE_TRY try
#define PHP_CORE_CATCH catch(const phpext::exception& e) {                  \
		zend_throw_exception(zend_ce_exception, e.what(), e.code());      \
		return;                                                           \
	}catch(const std::exception& e){                                      \
		zend_throw_exception(zend_ce_exception, e.what(), 0);             \
		return;                                                           \
	}catch(...){                                                          \
		zend_throw_exception(zend_ce_exception, "unknown exception", -1); \
		return;                                                           \
	}


namespace phpext {
	namespace internal {
		static char _zero[2048] = {0};
		static VARIABLE_IS_NOT_USED void* zero = _zero;
	}
}