#include "property_entry.h"
#include "value.h"
#include "exception.h"

namespace php {
    // 属性定义，可选文档注释
    property_entry::property_entry(std::string_view key, const php::value& val, std::uint32_t v, zend_string* doc)
        : name(zend_string_init_interned(key.data(), key.size(), true))
        , v(v), comment(doc) {

        switch(val.type()) {
        case TYPE_NULL:
        case TYPE_TRUE:
        case TYPE_FALSE:
        case TYPE_INTEGER:
        case TYPE_FLOAT:
            ZVAL_COPY_VALUE(&value, val);
            break;
        case TYPE_STRING:
            {
                zend_string* str = zend_string_dup(val, true);
                // 注意：测试中发现仅 INTERNED STRING 可用于 ATTRIBUTE 参数
                ZVAL_INTERNED_STR(&value, zend_new_interned_string(str));
                zend_string_free(str);
            }
            break;
        case TYPE_ARRAY:
            ZVAL_ARR(&value, zend_array_dup(val));
            GC_TYPE_INFO(Z_ARR(value)) = GC_ARRAY | ((IS_ARRAY_IMMUTABLE|GC_NOT_COLLECTABLE) << GC_FLAGS_SHIFT);
            break;
        default: {
                std::string message = "property can only be of basic types, '";
                message.append(zend_zval_type_name(val));
                message.append("' given");
                throw php::type_error(message);
            }
        }
    }

    void property_entry::do_register(std::vector<property_entry>& props, zend_class_entry* ce) {
        for(auto& p : props) // 属性注册
            zend_declare_property_ex(ce, p.name, &p.value, p.v, p.comment);
        props.clear(); // 上述声明过程内自行构造了 property 结构，但引用对应数据
    }
}
