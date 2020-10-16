#include "property_refer.h"
#include "value.h"

namespace php {
    zval* property_refer_traits::pointer(const property_refer* v) {
        return v->refer_;
    }
    void property_refer_traits::pointer(const property_refer* v, zval* z) {
        v->refer_ = z;
    }
    // 赋值
    property_refer& property_refer::operator =(const value& v) {
        zval_ptr_dtor(value_traits::pointer(this));
        ZVAL_COPY(value_traits::pointer(this), php::value_traits::pointer(&v));
        return *this;
    }
    // 赋值
    property_refer& property_refer::operator =(value&& v) {
        zval_ptr_dtor(value_traits::pointer(this));
        ZVAL_COPY_VALUE(value_traits::pointer(this), php::value_traits::pointer(&v));
        ZVAL_UNDEF(php::value_traits::pointer(&v));
        return *this;
    }
}
