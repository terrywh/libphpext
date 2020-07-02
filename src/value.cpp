#include "value.h"
#include "class_entry.h"
#include "callback.h"

namespace php {
    zval* value_traits::pointer(const value* v) {
        return &v->value_;
    }
    // 构造回调
    value::value(std::function<value (parameters& params)> cb) {
        object_init_ex(&value_, class_entry<callback>::entry());
        class_entry<callback>::native(Z_OBJ(value_))->fn(cb);
    }
}
