#ifndef LIBPHPEXT_EXCEPTION_H
#define LIBPHPEXT_EXCEPTION_H

#include "vendor.h"

namespace php {
    // 抛出的异常对象，不进行释放
    class throwable: public std::exception {
    public:
        // 构建 PHP 异常对象
        static void init(zval* self, zend_class_entry* ce, std::string_view message, int code = 0);
        // 空构建
        throwable() {
            ZVAL_UNDEF(&value_);
        }
        // 标准异常消息输出
        const char* what() const noexcept override;
        // 隐式转换
        operator zval*() const {
            return &value_;
        }
    protected:
        throwable(zend_object* ex) {
            ZVAL_OBJ(&value_, ex);
        }
        // 对象容器
        mutable zval value_;
    };
    // 异常：一般异常
    class exception: public throwable {
    public:
        using throwable::throwable;
        exception(std::string_view message, int code = 0) {
            throwable::init(&value_, zend_ce_exception, message, code); }
    protected:
        exception(zend_object* obj) : throwable(obj) { }
        friend void try_rethrow();
    };
    // 异常：错误异常
    class error_exception: public exception {
    public:
        error_exception(std::string_view message, int code = 0) {
            throwable::init(&value_, zend_ce_error_exception, message, code); }
    protected:
        error_exception(zend_object* obj) : exception(obj) { }
        friend void try_rethrow();
    };
    // 异常：错误
    class error: public throwable {
    public:
        using throwable::throwable;
        error(std::string_view message, int code = 0) {
            throwable::init(&value_, zend_ce_error, message, code); }
    protected:
        error(zend_object* obj) : throwable(obj) { }
        friend void try_rethrow();
    };
    // 异常：编译错误
    class compile_error: public error {
    public:
        using error::error;
        compile_error(std::string_view message, int code = 0) {
            throwable::init(&value_, zend_ce_compile_error, message, code); }
    protected:
        compile_error(zend_object* obj) : error(obj) { }
        friend void try_rethrow();
    };
    // 异常：解析错误
    class parse_error: public compile_error {
    public:
        parse_error(std::string_view message, int code = 0) {
            throwable::init(&value_, zend_ce_parse_error, message, code); }
    protected:
        parse_error(zend_object* obj) : compile_error(obj) { }
        friend void try_rethrow();
    };
    // 异常：类型错误
    class type_error: public error {
    public:
        using error::error;
        type_error(std::string_view message, int code = 0) {
            throwable::init(&value_, zend_ce_type_error, message, code); }
    protected:
        type_error(zend_object* obj) : error(obj) { }
        friend void try_rethrow();
    };
    // 异常：参数数量错误
    class argument_count_error: public type_error {
    public:
        argument_count_error(std::string_view message, int code = 0) {
            throwable::init(&value_, zend_ce_argument_count_error, message, code); }
    protected:
        argument_count_error(zend_object* obj) : type_error(obj) { }
        friend void try_rethrow();
    };
    // 异常：计算错误
    class value_error: public error {
    public:
        value_error(std::string_view message, int code = 0) {
            throwable::init(&value_, zend_ce_value_error, message, code); }
    protected:
        value_error(zend_object* obj) : error(obj) { }
        friend void try_rethrow();
    };
    // 异常：计算错误
    class arithmetic_error: public error {
    public:
        using error::error;
        arithmetic_error(std::string_view message, int code = 0){
            throwable::init(&value_, zend_ce_arithmetic_error, message, code); }
    protected:
        arithmetic_error(zend_object* obj) : error(obj) { }
        friend void try_rethrow();
    };
    // 异常：除 0 错误
    class division_by_zero_error: public arithmetic_error {
    public:
        division_by_zero_error(std::string_view message, int code = 0) {
            throwable::init(&value_, zend_ce_division_by_zero_error, message, code); }
    protected:
        division_by_zero_error(zend_object* obj) : arithmetic_error(obj) { }
        friend void try_rethrow();
    };
    // 将 CPP 中捕获的异常重新抛出到 PHP 中
    inline void rethrow(const throwable& ex) {
        zend_throw_exception_object(ex);
    }
    // 将 PHP 中发生的异常（若存在）重新抛出到 CPP 中
    void try_rethrow();
}

#endif // LIBPHPEXT_EXCEPTION_H
