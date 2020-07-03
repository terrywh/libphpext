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
    // 计算 T/t G/g M/m K/k 单位
    std::int64_t to_bytes(std::string_view str) {
        std::int64_t size = std::strtoul(str.data(), nullptr, 10);
        std::size_t  npos = str.find_last_not_of(' ');
        if(npos != str.npos) {
            switch(str[npos]) {
            case 't':
            case 'T':
                size *= 1024;
            case 'g':
            case 'G':
                size *= 1024;
            case 'm':
            case 'M':
                size *= 1024;
            case 'k':
            case 'K':
                size *= 1024;
            }
        }
        return size;
    }
}
