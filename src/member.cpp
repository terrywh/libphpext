#include "member.h"
#include "value.h"

namespace php {
    zval* member_traits::pointer(const member* v) {
        return v->refer_;
    }
    void member_traits::pointer(const member* v, zval* z) {
        v->refer_ = z;
    }
    // 赋值
    member& member::operator =(const value& v) {
        zval_ptr_dtor(value_traits::pointer(this));
        ZVAL_COPY(value_traits::pointer(this), php::value_traits::pointer(&v));
        return *this;
    }
    // 赋值
    member& member::operator =(value&& v) {
        zval_ptr_dtor(value_traits::pointer(this));
        ZVAL_COPY_VALUE(value_traits::pointer(this), php::value_traits::pointer(&v));
        ZVAL_UNDEF(php::value_traits::pointer(&v));
        return *this;
    }
}
