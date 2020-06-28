#ifndef LIBPHPEXT_STRING_H
#define LIBPHPEXT_STRING_H

#include "vendor.h"
#include "type_code.h"

namespace php {
    template <bool RAU>
    class value_t;
    using value = value_t<true>;

    class string_builder;
    // PHP 字符串
    class string: public zend_string {
    public:
        // 字符串类型
        static type_code_t TYPE_CODE;
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
    // 使用 smart_str 构建字符串（相较 string_buffer 更为轻量，效率更高）
    class string_builder {
    public:
        // 构建缓冲区
        string_builder()
        : str_ {nullptr, 0} {

        }
        // 追加字符
        string_builder& push_back(char c) {
            smart_str_appendc_ex(&str_, c, false);
            return *this;
        }
        string_builder& append(char c) {
            smart_str_appendc_ex(&str_, c, false);
            return *this;
        }
        string_builder& append(int v) {
            smart_str_append_long_ex(&str_, v, false);
            return *this;
        }
        string_builder& append(uint v) {
            smart_str_append_unsigned_ex(&str_, v, false);
            return *this;
        }
        string_builder& append(std::int64_t v) {
            smart_str_append_long_ex(&str_, v, false);
            return *this;
        }
        string_builder& append(std::uint64_t v) {
            smart_str_append_unsigned_ex(&str_, v, false);
            return *this;
        }
        // 追加文本
        string_builder& append(std::string_view s) {
            smart_str_appendl_ex(&str_, s.data(), s.size(), false);
            return *this;
        }
        // 获取构建的字符串
        zend_string* str() {
            assert(str_.s); // 保证字符串存在
            smart_str_0(&str_); // 保证 \0 结束
            zend_string* s = str_.s;
            // 当前 buffer 需要清空
            memset(&str_, 0, sizeof(smart_str));
            return s;
        }
    private:
        smart_str str_;

        friend string_builder& operator << (string_builder& sb, const value& data);
    };
    // PHP 数据序列化（JSON）
    string_builder& operator << (string_builder& sb, const value& data);
}

#endif // LIBPHPEXT_STRING_H
