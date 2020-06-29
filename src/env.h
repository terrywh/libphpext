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
        // 未定义引用
        static value undefined_value;
        // 空值引用
        static value null_value;
        // 文本常量：方法名称
        static value key(method_name mn);
        // 文本常量：内部名称（属性名、方法名、类名等，也可考虑在声明时设置引用获取）
        static value key(std::string_view name);
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
        };
    private:
        // 环境数据初始化
        static void init();
        friend class module_entry;
    };
}

#endif // LIBPHPEXT_CONSTANTS_H
