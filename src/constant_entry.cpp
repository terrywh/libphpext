#include "constant_entry.h"
#include "value.h"
#include "module_entry.h"

namespace php {
    // // 常量定义，可选文档注释
	// constant_entry::constant_entry(std::string_view key, const ::php::value& val, zend_string* doc)
	// : comment(doc) {
    //     name = zend_string_init_interned(key.data(), key.size(), true);
    //     DO_CONSTANT_VALUE();
    // }
    // 常量定义，可选文档注释
	constant_entry::constant_entry(std::string_view key, const ::php::value& val)
	/*: comment(nullptr) */ {
        name = zend_string_init_interned(key.data(), key.size(), true);
        switch(val.type()) {
        case TYPE_NULL:
        case TYPE_TRUE:
        case TYPE_FALSE:
        case TYPE_INTEGER:
        case TYPE_FLOAT:
            ZVAL_COPY_VALUE(&value, val);
            break;
        case TYPE_STRING: {
            zend_string* str = zend_string_dup(val, true);
            ZVAL_INTERNED_STR(&value, zend_new_interned_string(str));
            zend_string_free(str);
        }
            break;
        case TYPE_ARRAY:
            ZVAL_ARR(&value, zend_array_dup(val));
            GC_TYPE_INFO(Z_ARR(value)) = GC_ARRAY | ((IS_ARRAY_IMMUTABLE|GC_NOT_COLLECTABLE) << GC_FLAGS_SHIFT);
            break;
        default: {
            std::string message = "constant can only be of basic types, '";
            message.append(zend_zval_type_name(val));
            message.append("' given");
            throw php::type_error(message);
        }
        }
    }
    // 模块常量注册
    void constant_entry::do_register(std::vector<constant_entry>& entry, int module) {
        for(auto& c : entry) {
            ZEND_CONSTANT_SET_FLAGS(&c, CONST_PERSISTENT, module);
            zend_register_constant(&c);
        }
        entry.clear();
    }
    // 类常量注册
    void constant_entry::do_register(std::vector<constant_entry>& entry, zend_class_entry* ce) {
        for(auto& c : entry) // 常量注册
            zend_declare_class_constant_ex(ce, c.name, &c.value, ZEND_ACC_PUBLIC, /* c.comment */ nullptr);
        entry.clear();
    }
}
