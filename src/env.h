#ifndef LIBPHPEXT_CONSTANTS_H
#define LIBPHPEXT_CONSTANTS_H

#include "vendor.h"
#include "value.h"

namespace php {
    // 常量标识：方法名称
    enum class method_name {
        __CONSTRUCTOR,
        __DESTRUCTOR,
        __INVOKE,

        METHOD_NAME_MAX,
    };
    class env {
    public:
        // 文本常量：方法名称
        static value str(method_name mn);
        // 模拟一个函数形式进行 ini 读取
        class ini {
        public:
            // 待读取项
            ini(std::string_view name);
            operator std::int64_t() const;
            operator std::size_t() const;
            operator value() const;
            operator std::string() const;
            // 计算字节数量（单位）
            std::int64_t bytes() const;
            bool exists() const {
                return !!entry_;
            }
        private:
            zend_ini_entry* entry_;
            // TODO ini 数据读取功能 https://github.com/terrywh/libphpext/blob/master/src/ini.cpp
        };
    };
}

#endif // LIBPHPEXT_CONSTANTS_H
