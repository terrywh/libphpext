#include "constant.h"

namespace php {
    // 文本常量：方法名称
    static const char* method_name_cstr[] = {
        ZEND_CONSTRUCTOR_FUNC_NAME,
        ZEND_DESTRUCTOR_FUNC_NAME,
        ZEND_INVOKE_FUNC_NAME,
    };
    // 文本常量：方法名称
    value constant::string(method_name mn) {
        static value str[static_cast<int>(method_name::METHOD_NAME_MAX)];
        int i = static_cast<int>(mn);
        // 若常量还未创建
        if(!str[i].is(TYPE_STRING)) {
            const char* s = method_name_cstr[i];
            // 创建内部字符串
            str[i] = zend_string_init_interned(s, std::strlen(s), true);
        }
        return str[i];
    }
}