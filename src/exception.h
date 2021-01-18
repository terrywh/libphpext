#ifndef LIBPHPEXT_EXCEPTION_H
#define LIBPHPEXT_EXCEPTION_H

#include "vendor.h"

namespace php {
    class array;
    // 抛出的异常对象
    // 注意：异常对象不在 C++ 侧进行内存控制管理（由 PHP Zend 引擎负责释放）
    class throwable: public std::exception {
    public:
        throwable(zend_object* ex);
        // 标准异常消息输出
        const char* what() const noexcept override;
    protected:
        throwable(zend_class_entry* ce, std::string_view message, int code = 0)
        : ce_(ce), message_(message), code_(code) {
            // ZVAL_OBJ(&value_, ex);
        }
        // 基类 (exception / error)
        // zend_class_entry* base_class_entry() const;
        // 对象容器
        // mutable zval value_;
        zend_class_entry* ce_;
        std::string message_;
        int code_;

        friend void php_rethrow(const std::exception& ex);
        friend void cpp_rethrow();
    };
    // 异常：一般异常
    class exception: public throwable {
    public:
        exception(std::string_view message, int code = 0)
        : throwable(zend_ce_exception, message, code) {}
        using throwable::throwable;
        
    };
    // 异常：错误异常
    class error_exception: public exception {
    public:
        error_exception(std::string_view message, int code = 0)
        : exception(zend_ce_error, message, code) {}
        using exception::exception;
    };
    // 异常：错误
    class error: public throwable {
    public:
        error(std::string_view message, int code = 0)
        : throwable(zend_ce_error, message, code) {}
        using throwable::throwable;
    };
    // 异常：编译错误
    class compile_error: public error {
    public:
        compile_error(std::string_view message, int code = 0)
        : error(zend_ce_compile_error, message, code) {}
        using error::error;
    };
    // 异常：解析错误
    class parse_error: public compile_error {
    public:
        parse_error(std::string_view message, int code = 0)
        : compile_error(zend_ce_parse_error, message, code) {}
        using compile_error::compile_error;
    };
    // 异常：类型错误
    class type_error: public error {
    public:
        type_error(std::string_view message, int code = 0)
        : error(zend_ce_type_error, message, code) {}
        using error::error;
    };
    // 异常：参数数量错误
    class argument_count_error: public type_error {
    public:
        argument_count_error(std::string_view message, int code = 0)
        : type_error(zend_ce_argument_count_error, message, code) {}
        using type_error::type_error;
    };
    // 异常：计算错误
    class value_error: public error {
    public:
        value_error(std::string_view message, int code = 0)
        : error(zend_ce_value_error, message, code) { }
        using error::error;
    };
    // 异常：计算错误
    class arithmetic_error: public error {
    public:
        arithmetic_error(std::string_view message, int code = 0)
        : error(zend_ce_arithmetic_error, message, code) { }
        using error::error;
    };
    // 异常：除 0 错误
    class division_by_zero_error: public arithmetic_error {
    public:
        division_by_zero_error(std::string_view message, int code = 0)
        : arithmetic_error(zend_ce_division_by_zero_error, message, code) { }
        using arithmetic_error::arithmetic_error;
    };
    // 将 CPP 中捕获的异常重新抛出到 PHP 中
    inline void php_rethrow(const std::exception& ex) {
        try {
            const php::throwable& e = dynamic_cast<const php::throwable&>(ex);
            zend_throw_exception(e.ce_, e.what(), e.code_);
        }
        catch(const std::bad_cast& e) {
            zend_throw_exception(zend_ce_error, ex.what(), -1);
        }
    }
    // 将 PHP 中发生的异常（若存在）重新抛出到 CPP 中
    void cpp_rethrow();
    // 存在异常或严重错误
    inline bool has_error() {
        return EG(exception) != nullptr || (PG(last_error_type) & E_FATAL_ERRORS) != 0;
    }
}

#endif // LIBPHPEXT_EXCEPTION_H
