#ifndef LIBPHPEXT_PROPERTY_ENTRY_H
#define LIBPHPEXT_PROPERTY_ENTRY_H

#include "vendor.h"
#include "value.h"

namespace php {
    // 属性定义
    class property_entry: public zend_constant {
    public:
        // 属性定义，可选文档注释
        property_entry(zend_string* key, const php::value& val, std::uint32_t v, zend_string* doc)
        : name(key), v(v), comment(doc) {
            if(val.is(TYPE_STRING)) {
                std::string_view sv = val;
                ZVAL_INTERNED_STR(&value, zend_string_init_interned(sv.data(), sv.size(), true));
            }
            else
                ZVAL_COPY(&value, val); // 实际未销毁
        }
        // 结构定义参考 zend_constant
        zval           value;
        zend_string*    name;
        std::uint32_t      v; // visibility
        zend_string* comment;
    };
}

#endif // LIBPHPEXT_PROPERTY_ENTRY_H
