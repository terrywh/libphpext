#ifndef LIBPHPEXT_STRING_BUILDER_H
#define LIBPHPEXT_STRING_BUILDER_H

#include "vendor.h"
#include "type_code.h"

namespace php {
    class value;
    // 字符串构建工具
    // 使用 smart_str 构建字符串（相较 string_buffer 更为轻量，相对效率更高）
    class string_builder {
    public:
        // 构建缓冲区
        string_builder()
        : str_ {nullptr, 0} { }
        // 释放缓冲区
        ~string_builder() {
            smart_str_free_ex(&str_, false);
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
        value str();
    private:
        smart_str str_;

        friend string_builder& operator << (string_builder& sb, const value& data);
    };
    // PHP 数据序列化（JSON）
    string_builder& operator << (string_builder& sb, const value& data);
}

#endif // LIBPHPEXT_STRING_H
