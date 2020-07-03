#include "constant_entry.h"
#include "value.h"
#include "module_entry.h"

namespace php {
    // 仅允许基础类型（布尔+数值+字符串）
#define DO_CONSTANT_VALUE() do {                                                            \
    assert(!Z_REFCOUNTED_P(static_cast<zval*>(val)) || val.is(TYPE_STRING));              \
    if(val.is(TYPE_STRING)) {                                                             \
        std::string_view vs = val;                                                        \
        ZVAL_INTERNED_STR(&value, zend_string_init_interned(vs.data(), vs.size(), true)); \
    }                                                                                     \
    else ZVAL_COPY_VALUE(&value, val);                                                    \
} while(false)
    // 常量定义，可选文档注释
	constant_entry::constant_entry(zend_string* key, const ::php::value& val, zend_string* doc)
	: comment(doc) {
        name = key;
        DO_CONSTANT_VALUE();
    }
    // 常量定义，可选文档注释
	constant_entry::constant_entry(zend_string* key, const ::php::value& val)
	: comment(nullptr) {
        name = key;
        DO_CONSTANT_VALUE();
        // 扩展常量额外的标记
        ZEND_CONSTANT_SET_FLAGS(this, CONST_PERSISTENT, module_entry::self()->module);
    }
}
