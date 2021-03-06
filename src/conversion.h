#ifndef LIBPHPEXT_CONVERSION_H
#define LIBPHPEXT_CONVERSION_H

#include "vendor.h"

namespace php {
    class value;
    // 转换（复制）一个布尔
    value to_boolean(const value& val);
    // 转换（复制）一个整数
    value to_integer(const value& val, int base = 10);
    // 转换（复制）一个浮点数
    value to_double(const value& val);
    // 转换（复制）一个字符串
    value to_string(const value& val);
    // 转换（复制）一个数组
    value to_array(const value& val);
    // 转换（复制）一个对象
    value to_object(const value& val);
}

#endif // LIBPHPEXT_CONVERT_H