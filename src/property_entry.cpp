#include "property_entry.h"
#include "value.h"

namespace php {
    // 属性定义，可选文档注释
    property_entry::property_entry(zend_string* key, const php::value& val, std::uint32_t v, zend_string* doc)
        : name(key), v(v), comment(doc) {
            if(val.is(TYPE_STRING)) {
                std::string_view sv = val;
                ZVAL_INTERNED_STR(&value, zend_string_init_interned(sv.data(), sv.size(), true));
            }
            else
                ZVAL_COPY(&value, val); // 实际未销毁
        }
}
