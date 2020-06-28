#include "array.h"
#include "value.h"

namespace php {
    // 数组类型
    type_code_t array::TYPE_CODE = TYPE_ARRAY;
    // 数组元素访问
    value& array::operator [](int idx) const {
        // TODO
    }
    // 数组元素访问
    value& array::operator [](const value& idx) const {
        // TODO
    }
    // TODO
}