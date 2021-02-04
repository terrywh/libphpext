#ifndef LIBPHPEXT_VALUE_BASIC_H
#define LIBPHPEXT_VALUE_BASIC_H

#include "basic_value.h"

namespace php {
    // PHP 变量（自动释放）
    struct basic_value_traits {
        static inline zval* ptr(const value* v) {
            return reinterpret_cast<zval*>(const_cast<value*>(v));
        }
    };
    class value: public basic_value<value, basic_value_traits> {
    private:
        mutable zval value_;
    public:
        using value_traits = basic_value_traits;
        // 构建：未定义
        value() {
            ZVAL_UNDEF(&value_);
        }
        ~value() {
            zval_ptr_dtor(&value_);
        }
        using basic_value::basic_value;
        OVERRIDE_IMPLICIT_DECLARATION(value);
        
        friend struct basic_value_traits;
    };
    // 确认包裹大小一致（可以进行引用）
    static_assert(sizeof(value) == sizeof(zval));
}

#endif // LIBPHPEXT_VALUE_BASIC_H
