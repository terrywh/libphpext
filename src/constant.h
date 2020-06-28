#ifndef LIBPHPEXT_CONSTANTS_H
#define LIBPHPEXT_CONSTANTS_H

#include "vendor.h"
#include "value.h"

namespace php {

    class constant {
    public:
        // 常量标识：方法名称
        enum class method_name {
            __CONSTRUCTOR,
            __DESTRUCTOR,
            __INVOKE,

            METHOD_NAME_MAX,
        };
        // 文本常量：方法名称
        static value string(method_name mn);
    };
}

#endif // LIBPHPEXT_CONSTANTS_H
