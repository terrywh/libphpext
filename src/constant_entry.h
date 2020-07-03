#ifndef LIBPHPEXT_CONSTANT_ENTRY_H
#define LIBPHPEXT_CONSTANT_ENTRY_H

#include "vendor.h"

namespace php {
    class value;
    // 常量定义
    class constant_entry: public zend_constant {
    public:
        // 模块常量定义
        constant_entry(zend_string* key, const php::value& val);
        // 类常量定义，可选文档注释
        constant_entry(zend_string* key, const php::value& val, zend_string* doc);
        // 添加成员
        zend_string* comment;
    };
}

#endif // LIBPHPEXT_CONSTANT_ENTRY_H
