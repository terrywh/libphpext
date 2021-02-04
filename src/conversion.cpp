#include "conversion.h"
#include "value.h"

namespace php {
    // 转换（复制）一个布尔
    value to_boolean(const value& val) {
        value rv;
        ZVAL_COPY(rv, val);
        convert_to_boolean(rv);
        return rv;
    }
    // 转换（复制）一个整数
    value to_integer(const value& val, int base) {
        value rv;
        ZVAL_COPY(rv, val);
        convert_to_long_base(rv, base);
        return rv;
    }
    value to_double(const value& val) {
        value rv;
        ZVAL_COPY(rv, val);
        convert_to_double(rv);
        return rv;
    }
    // 转换（复制）一个字符串
    value to_string(const value& val) {
        value rv;
        ZVAL_COPY(rv, val);
        _convert_to_string(rv);
        return rv;
    }
    // 转换（复制）一个数组
    value to_array(const value& val) {
        value rv;
        ZVAL_COPY(rv, val);
        convert_to_array(rv);
        return rv;
    }
    // 转换（复制）一个对象
    value to_object(const value& val) {
        value rv;
        ZVAL_COPY(rv, val);
        convert_to_object(rv);
        return rv;
    }
}
