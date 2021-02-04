#include "property_refer.h"

namespace php {
    zval* property_refer_traits::ptr(const property_refer* v) {
        return v->refer_;
    }
    void property_refer_traits::ptr(const property_refer* v, zval* z) {
        v->refer_ = z;
    }
}
