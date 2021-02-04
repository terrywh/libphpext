#ifndef LIBPHPEXT_PROPERTY_REFER_H
#define LIBPHPEXT_PROPERTY_REFER_H

#include "vendor.h"
#include "value.h"

namespace php {
    class property_refer;
    struct property_refer_traits {
        //
        static zval* ptr(const property_refer* v);
        // 特化用于同步指向
        static void  ptr(const property_refer* v, zval* z);
    };
    // C++ 属性（同步快速访问）
    class property_refer: public basic_value<property_refer, property_refer_traits> {
    private:
        mutable zval* refer_;
    public:
        using value_traits = property_refer_traits;
        //
        property_refer()
        // 注意：基类默认构造需要检查
        : refer_(nullptr) { }
        ~property_refer() {
            // 不做释放操作
        }
        OVERRIDE_IMPLICIT_DECLARATION(property_refer);
        // 转换 value 通用类型
        operator value();
        // 赋值
        property_refer& operator =(const value& v); // 赋值
        property_refer& operator =(value&& v); // 赋值
        
        friend struct property_refer_traits;
    };
}

#endif //LIBPHPEXT_PROPERTY_REFER_H
