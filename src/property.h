#ifndef LIBPHPEXT_PROPERTY_H
#define LIBPHPEXT_PROPERTY_H

#include "value_basic.h"

namespace php {
    class  value;
    class  property;
    struct property_traits {
        static zval* pointer(const property* v);
    };
    // 对象属性（不增加引用，存在动态计算故重定义赋值操作）
    class property: public value_basic<property, property_traits> {
    private:
        using value_traits = property_traits;
        mutable zval value_;
    public:
        ~property() {
            // 不做释放操作
        }
        OVERRIDE_IMPLICIT_DECLARATION(property);
        // 构建对象(读取赋值属性值）
        property(zend_object* obj, zend_string* key);
        // 转换 value 通用类型
        operator value();
        // 赋值
        property& operator =(const value& v);
        // 不支持 移动赋值
        // 运算符（存在缓存）
#define DECLARE_OPERATOR(TYPE, OPR) property& operator OPR(TYPE x)
        property& operator ++();
        property& operator --();
        DECLARE_OPERATOR(int,+=);
        DECLARE_OPERATOR(int,-=);
        DECLARE_OPERATOR(int,*=);
        DECLARE_OPERATOR(int,/=);
        DECLARE_OPERATOR(std::int64_t,+=);
        DECLARE_OPERATOR(std::int64_t,-=);
        DECLARE_OPERATOR(std::int64_t,*=);
        DECLARE_OPERATOR(std::int64_t,/=);
        DECLARE_OPERATOR(float,+=);
        DECLARE_OPERATOR(float,-=);
        DECLARE_OPERATOR(float,*=);
        DECLARE_OPERATOR(float,/=);
        DECLARE_OPERATOR(double,+=);
        DECLARE_OPERATOR(double,-=);
        DECLARE_OPERATOR(double,*=);
        DECLARE_OPERATOR(double,/=);
#undef DECLARE_OPERATOR
    private:
        // 属主
        zend_object* obj_;
        // 键名
        zend_string* key_;

        friend struct property_traits;
    };
}

#endif //LIBPHPEXT_PROPERTY_H
