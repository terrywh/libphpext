#ifndef LIBPHPEXT_CONSTANTS_H
#define LIBPHPEXT_CONSTANTS_H

#include "forward.h"
#include "value.h"

namespace php {
    // 方法名称下标
    enum class  method_name {
        __CONSTRUCTOR,
        __DESTRUCTOR,
        __INVOKE,

        METHOD_NAME_MAX,
    };
    // 实际文本定义
    const char* method_name_s[] = {
        ZEND_CONSTRUCTOR_FUNC_NAME,
        ZEND_DESTRUCTOR_FUNC_NAME,
        ZEND_INVOKE_FUNC_NAME,
    };
    // 文本获取：方法名称
    value cstr(method_name mn) {
        static value str[static_cast<int>(method_name::METHOD_NAME_MAX)];
        int i = static_cast<int>(mn);
        const char* s = method_name_s[i];
        if(!str[i].is(value::TYPE_STRING)) {
            str[i] = zend_string_init_interned(s, std::strlen(s), true);
        }
        return str[i];
    }
}

#endif // LIBPHPEXT_CONSTANTS_H
