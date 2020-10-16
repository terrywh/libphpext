#ifndef LIBPHPEXT_ATTRIBUTE_ENTRY_H
#define LIBPHPEXT_ATTRIBUTE_ENTRY_H

#include "vendor.h"
#include "value.h"

namespace php {
    class attribute_entry {
    public:
        attribute_entry(std::string_view name, std::vector<php::value> argv)
        : name_(zend_string_init_interned(name.data(), name.size(), true))
        , argv_(std::move(argv)) {

        }
        attribute_entry()
        : name_(zend_string_init_interned("Attribute", sizeof("Attribute")-1, true)) {

        }
        // 注册或写入 ATTRIBUTE 元数据
        // 注意：数组 *attributes 会自动构建
        void finalize(zend_array** attributes);
    private:
        zend_string*       name_;
        std::vector<value> argv_;
    };
}

#endif // LIBPHPEXT_ATTRIBUTE_ENTRY_H
