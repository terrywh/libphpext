#ifndef LIBPHPEXT_PROPERTY_H
#define LIBPHPEXT_PROPERTY_H

#include "value.h"
namespace php {
    // 对象属性（不增加引用，存在动态计算故重定义赋值操作）
    class property: public value_t<false> {
    public:
        // 构建对象(读取赋值属性值）
        property(zend_object* obj, zend_string* key);
        // 赋值：复制
        property& operator =(const value& v);
        // 赋值：移动
        property& operator =(value&& v);
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
    };
}

#endif //LIBPHPEXT_PROPERTY_H
