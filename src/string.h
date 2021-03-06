#ifndef LIBPHPEXT_STRING_H
#define LIBPHPEXT_STRING_H

#include "vendor.h"
#include "type.h"

namespace php {
    class value;

    class string_builder;
    // PHP 字符串
    class string: public zend_string {
    public:
        // 字符串类型
        static constexpr type_code TYPE_CODE = TYPE_STRING;
        
        // 开始构建字符串
        static string_builder build();
        // 
        inline std::size_t size() const {
            return ZSTR_LEN(this);
        }
        inline const char* c_str() const {
            return val;
        }
        inline char* data() {
            return val;
        }
        // 长度缩减
        void shrink(std::size_t len) {
            assert(len < ZSTR_LEN(this));
            ZSTR_LEN(this) = len;
            ZSTR_VAL(this)[len] = '\0';
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
