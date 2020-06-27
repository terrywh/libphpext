#ifndef LIBPHPEXT_EXCEPTION_H
#define LIBPHPEXT_EXCEPTION_H

#include "forward.h"
#include "value.h"
#include "object.h"

namespace php {
	// 抛出的异常对象，不进行释放
	class throwable: public value_t<false> {
    public:
        // using value_t<false>::value_t;
		const char* what() const noexcept;
	protected:
		throwable(zend_object* obj) {
			ZVAL_OBJ(&value_, obj);
		}
    };
	// 异常： 对应 zval -> zend_ce_exception
	class exception: public throwable {
	public:
		exception(zend_object* obj)
		: throwable(obj) {

		}
		// 构造异常
		exception(std::string_view message, int code = 0)
		: throwable(object::create(zend_ce_exception, {message, code})) {
			
		}
	};

	// 将 CPP 中捕获的异常重新抛出到 PHP 中
	inline void rethrow(const throwable& ex) {
		zend_throw_exception_object(ex);
	}
	// 将 PHP 中发生的异常重新抛出到 CPP 中
	void rethrow();
}

#endif // LIBPHPEXT_EXCEPTION_H
