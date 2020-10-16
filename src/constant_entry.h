#ifndef LIBPHPEXT_CONSTANT_ENTRY_H
#define LIBPHPEXT_CONSTANT_ENTRY_H

#include "vendor.h"

namespace php {
    class value;
    // 常量定义
    class constant_entry: public zend_constant {
    public:
        // 常量定义
        constant_entry(std::string_view key, const php::value& val);
        // 类常量定义，可选文档注释
        // constant_entry(std::string_view key, const php::value& val, zend_string* doc);
        // 模块常量注册
        static void do_register(std::vector<constant_entry>& entry, int module);
        // 类常量注册
        static void do_register(std::vector<constant_entry>& entry, zend_class_entry* ce);
    private:
        // zend_string* comment; // 类常量支持额外的文档注释
    };
}

#endif // LIBPHPEXT_CONSTANT_ENTRY_H
