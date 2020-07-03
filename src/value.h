#ifndef LIBPHPEXT_VALUE_H
#define LIBPHPEXT_VALUE_H

#include "vendor.h"
#include "value_basic.h"

namespace php {
    class property;
    class member;
    class parameters;

    class value;
    struct value_traits {
        static zval* pointer(const value* v);
    };
    // PHP 变量（自动释放）
    class value: public value_basic<value, value_traits> {
    private:
//        using value_traits = value_traits;
        mutable zval value_;
    public:
        // 构建：未定义
        value() {
            ZVAL_UNDEF(&value_);
        }
        ~value() {
            zval_ptr_dtor(&value_);
        }
        using value_basic::value_basic;
        OVERRIDE_IMPLICIT_DECLARATION(value);
        // 构造回调
        value(std::function<value (parameters& params)> fn);
        // 赋值
        template <class T, typename = typename T::value_traits>
        value& operator =(const T& v) {
            zval_ptr_dtor(value_traits::pointer(this));
            ZVAL_COPY(value_traits::pointer(this), T::value_traits::pointer(&v));
            return *this;
        }
        // 数组分离 (用于支持 COW 写时复制)
        value_basic& operator *() {
            SEPARATE_ARRAY(&value_);
            return *this;
        }
        friend struct value_traits;
    };
    // 确认包裹大小一致（可以进行引用）
    static_assert(sizeof(value) == sizeof(zval));
}

// 嵌入哈析函数，方便容器使用
namespace std {
    template<> struct hash<php::value> {
        typedef php::value argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept {
            assert(s.is(php::TYPE_STRING));
            return s.as<php::string>()->hashcode();
        }
    };
}

#endif // LIBPHPEXT_VALUE_H
