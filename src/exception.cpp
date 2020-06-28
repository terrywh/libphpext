#include "exception.h"
#include "object.h"

namespace php {

    const char* throwable::what() const noexcept {
        zval *prop, rv;
        zend_string *name = zend_string_init("message", sizeof("message")-1, 0);
        prop = zend_read_property_ex(
            instanceof_function(Z_OBJCE(value_), zend_ce_exception) ? zend_ce_exception : zend_ce_error,
            const_cast<zval*>(&value_), name, 1, &rv);
        zend_string_release_ex(name, 0);
        return Z_STRVAL_P(prop);
    }
	// 构造异常
	exception::exception(std::string_view message, int code)
	: throwable(object::create(zend_ce_exception, {message, code})) {
		
	}
	// 将 PHP 中发生的异常（若存在）重新抛出到 CPP 中
    void try_rethrow() {
        // TODO
		if(EG(exception) == nullptr) {
			
		}
		else if(EG(exception)->ce == zend_ce_exception) {
			throw php::exception(EG(exception));
		}
		else if(EG(exception)->ce == zend_ce_error_exception) {

		}
		else if(EG(exception)->ce == zend_ce_error) {

		}
		else if(EG(exception)->ce == zend_ce_type_error) {

		}
		else if(EG(exception)->ce == zend_ce_argument_count_error) {

		}
		else if(EG(exception)->ce == zend_ce_arithmetic_error) {

		}
		else if(EG(exception)->ce == zend_ce_division_by_zero_error) {

		}
	}
}