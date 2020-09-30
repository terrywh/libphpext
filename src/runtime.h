#ifndef LIBPHPEXT_RUNTIME_H
#define LIBPHPEXT_RUNTIME_H

#include "vendor.h"

namespace php {
    class value;
    // 常量标识：路径
    enum class path {
        CURRENT_WORKING_DIRECTORY,
        PHP_BINARY_FILE,
        PHP_LOADED_INI_FILE,
    };
    class runtime {
    public:
        // 未定义引用
        static value undefined_value;
        // 文本常量：路径
        static std::string_view path(path pn);
        // 文本常量：命令行
        static std::string_view cmd();
        // 常量获取
        static value& c(std::string_view name);
        // 变量获取
        static value& g(std::string_view name);
    private:
        // 数据初始化
        static void init();
        friend class module_entry;
    };
}

#endif // LIBPHPEXT_ENV_H
