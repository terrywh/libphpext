#include "exception.h"
#include "object.h"
#include "env.h"
#include "value.h"
#include "array.h"

namespace php {
    throwable::throwable(zend_object* ex) {
        ce_ = instanceof_function(ex->ce, zend_ce_exception) ? zend_ce_exception : zend_ce_error;
        zval rv, *prop;
        prop = zend_read_property_ex(ce_, ex, ZSTR_KNOWN(ZEND_STR_MESSAGE), 1, &rv);
        message_.assign(Z_STRVAL_P(prop));
        prop = zend_read_property_ex(ce_, ex, ZSTR_KNOWN(ZEND_STR_CODE), 1, &rv);
        code_ = Z_LVAL_P(prop);
    }
    // 构建 PHP 异常对象
    // void throwable::init(zval* self, zend_class_entry* ce, std::string_view message, int code ) {
    //     ZVAL_OBJ(self, object::create(ce));
    //     php::value argv[2] = { {message}, {code} };
    //     object::call(Z_OBJ_P(self), env::key(method_name::__CONSTRUCTOR), 2, reinterpret_cast<zval*>(argv));
    // }
    // 异常编号
    // const int throwable::code() const noexcept {
    //     zval *prop, rv;
    //     prop = zend_read_property_ex(base_class_entry(), const_cast<zval*>(&value_), ZSTR_KNOWN(ZEND_STR_CODE), 1, &rv);
    //     return Z_LVAL_P(prop);
    // }
    // 异常消息
    const char* throwable::what() const noexcept {
        // zval *prop, rv;
        // prop = zend_read_property_ex(base_class_entry(), const_cast<zval*>(&value_), ZSTR_KNOWN(ZEND_STR_MESSAGE), 1, &rv);
        // return Z_STRVAL_P(prop);
        return message_.c_str();
    }
    // 堆栈信息
    // array& throwable::trace() const {
    //     zval *prop, rv;
    //     prop = zend_read_property_ex(base_class_entry(), const_cast<zval*>(&value_), ZSTR_KNOWN(ZEND_STR_TRACE), 1, &rv);
    //     return *reinterpret_cast<array*>(Z_ARR_P(prop));
    // }
    // 基类 (exception / error)
    // zend_class_entry* throwable::base_class_entry() const {
    //     return instanceof_function(Z_OBJCE(value_), zend_ce_exception) ? zend_ce_exception : zend_ce_error;
    // }
    // 将 PHP 中发生的异常（若存在）重新抛出到 CPP 中
    void cpp_rethrow() {
        if(EG(exception) == nullptr) {
            // 无异常
        }
        else if(EG(exception)->ce == zend_ce_exception)
            throw php::exception(EG(exception));
        else if(EG(exception)->ce == zend_ce_error_exception)
            throw php::error_exception(EG(exception));
        else if(EG(exception)->ce == zend_ce_error)
            throw php::error(EG(exception));
        else if(EG(exception)->ce == zend_ce_compile_error)
            throw php::compile_error(EG(exception));
        else if(EG(exception)->ce == zend_ce_parse_error)
            throw php::parse_error(EG(exception));
        else if(EG(exception)->ce == zend_ce_type_error)
            throw php::type_error(EG(exception));
        else if(EG(exception)->ce == zend_ce_argument_count_error)
            throw php::argument_count_error(EG(exception));
        else if(EG(exception)->ce == zend_ce_value_error)
            throw php::value_error(EG(exception));
        else if(EG(exception)->ce == zend_ce_arithmetic_error)
            throw php::arithmetic_error(EG(exception));
        else if(EG(exception)->ce == zend_ce_division_by_zero_error)
            throw php::division_by_zero_error(EG(exception));
        else
            abort(); // 未知的异常类型
    }
}