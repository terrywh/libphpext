#ifndef LIBPHPEXT_MEMBER_H
#define LIBPHPEXT_MEMBER_H

#include "vendor.h"
#include "value_basic.h"

namespace php {
    class member;
    struct member_traits {
        static zval* pointer(const member* v);
        static void  pointer(const member* v, zval* z); // 特化用于同步指向
    };
    // C++ 属性（同步快速访问）
    class member: public value_basic<member, member_traits> {
    private:
        using value_traits = member_traits;
        mutable zval* refer_;
    public:
        member()
        // 注意：基类默认构造需要检查
        : refer_(nullptr) { }
        ~member() {
            // 不做释放操作
        }
        OVERRIDE_IMPLICIT_DECLARATION(member);
        // 转换 value 通用类型
        operator value();
        // 赋值
        member& operator =(const value& v); // 赋值
        member& operator =(value&& v); // 赋值
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
        friend struct member_traits;
    };
}

#endif //LIBPHPEXT_MEMBER_H
