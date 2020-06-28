#include "constant_entry.h"
#include "module_entry.h"

namespace php {
    // 常量定义，可选文档注释
	constant_entry::constant_entry(std::string_view key, const ::php::value& val, std::string_view doc) {
        name = zend_string_init_interned(key.data(), key.size(), true);
        // 仅允许基础类型（布尔+数值+字符串）
        assert(!Z_REFCOUNTED_P(static_cast<zval*>(val)) || val.is(TYPE_STRING));
        // 字符串需要特殊处理
        if(Z_REFCOUNTED_P(static_cast<zval*>(val))) {
            std::string_view vs = val;
            ZVAL_INTERNED_STR(&value, zend_string_init_interned(vs.data(), vs.size(), true));
        }
        else ZVAL_COPY_VALUE(&value, val);
        // 注释说明
        comment = doc.empty() ? nullptr
            : zend_string_init_interned(doc.data(), doc.size(), true);
        // 扩展尝试标志
        ZEND_CONSTANT_SET_FLAGS(this, CONST_PERSISTENT, module_entry::self()->module);
    }
}
