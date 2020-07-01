#ifndef LIBPHPEXT_STRING_H
#define LIBPHPEXT_STRING_H

#include "vendor.h"
#include "type_code.h"

namespace php {
    class value;

    class string_builder;
    // PHP 字符串
    class string: public zend_string {
    public:
        // 字符串类型
        static constexpr type_code_t TYPE_CODE = TYPE_STRING;
        // 创建指定长度的字符串
        static value create(std::size_t size, bool persist = false);
        // 创建字符串（复制）
        static value create(std::string_view str, bool persist = false);

        static string_builder build();
        // 
        inline std::size_t size() {
            return ZSTR_LEN(this);
        }
        // 获取字符串（复制）
        operator std::string() const {
            return {ZSTR_VAL(this), ZSTR_LEN(this)};
        }
        // 获取字符串（查看）
        operator std::string_view() const {
            return {ZSTR_VAL(this), ZSTR_LEN(this)};
        }
        // 字符串哈析
        inline std::uint64_t hashcode() {
            return zend_string_hash_func(this);
        }
    };
}

#endif // LIBPHPEXT_STRING_H
