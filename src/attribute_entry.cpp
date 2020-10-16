#include "attribute_entry.h"
#include "exception.h"

namespace php {

    void attribute_entry::finalize(zend_array** attributes) {
            zend_attribute* attr = zend_add_attribute(attributes, name_, argv_.size(), ZEND_ATTRIBUTE_PERSISTENT, 0, 0);
            for(int i=0;i<argv_.size();++i) {
                switch(argv_[i].type_code()) {
                case TYPE_NULL:
                case TYPE_TRUE:
                case TYPE_FALSE:
                case TYPE_INTEGER:
                case TYPE_FLOAT:
                    ZVAL_COPY_VALUE(&attr->args[i].value, argv_[i]);
                    break;
                case TYPE_STRING:
                    {
                        zend_string* str = zend_string_dup(argv_[i], true);
                        // 注意：测试中发现仅 INTERNED STRING 可用于 ATTRIBUTE 参数
                        ZVAL_INTERNED_STR(&attr->args[i].value, zend_new_interned_string(str));
                        zend_string_free(str);
                    }
                    break;
                // case TYPE_ARRAY:
                //     ZVAL_ARR(&value, zend_array_dup(val));
                //     GC_TYPE_INFO(Z_ARR(value)) = GC_ARRAY | ((IS_ARRAY_IMMUTABLE|GC_NOT_COLLECTABLE) << GC_FLAGS_SHIFT);
                //     break;
                default: {
                        std::string message = "attribute argument can only be of basic types, '";
                        message.append(zend_zval_type_name(argv_[i]));
                        message.append("' given");
                        throw php::type_error(message);
                    }
                }
            }
        }
}