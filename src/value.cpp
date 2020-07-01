#include "value.h"
#include "property.h"
#include "member.h"

namespace php {
    zval* value_traits::pointer(const value* v) {
        return &v->value_;
    }
}
