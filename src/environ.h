#ifndef LIBPHPEXT_ENV_H
#define LIBPHPEXT_ENV_H

#include "vendor.h"

namespace php {
    class value;
    // 常量标识：方法名称
    enum class method_name {
        __CLONE,
        __CONSTRUCTOR,
        __DESTRUCTOR,
        __GET,
        __SET,
        __UNSET,
        __ISSET,
        __CALL,
        __CALLSTATIC,
        __TOSTRING,
        __INVOKE,
        __DEBUGINFO,

        METHOD_NAME_MAX,
    };
    class environ {
    public:
        // 文本常量：方法名称
        static zend_string* key(method_name mn);
        // 文本常量：内部名称（属性名、方法名、类名等，也可考虑在声明时设置引用获取）
        static zend_string* key(std::string_view name);
        // 常量获取
        static value& c(std::string_view name);
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
            std::int64_t byte_size() const;
            bool exists() const {
                return !!entry_;
            }
        private:
            zend_ini_entry* entry_;
        };


    private:
        // 环境数据初始化
        static void init();
        friend class module_entry;
    };
}

#endif // LIBPHPEXT_ENV_H
