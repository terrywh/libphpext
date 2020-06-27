#ifndef LIBPHPEXT_STRING_H
#define LIBPHPEXT_STRING_H

#include "forward.h"
#include "value.h"

namespace php {
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

    class string: public zend_string {
    public:
        // 对应类型 参见 value::as<string>()
        static value::types TYPE;
        // 创建指定长度的字符串
        static string* create(std::size_t size, bool persist = false);
        // 创建字符串（复制）
        static string* create(std::string_view str, bool persist = false);

        static string_builder build() {
            return string_builder();
        }
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
// 嵌入哈析函数，方便容器使用
namespace std {
    template<> struct hash<php::value> {
        typedef php::value argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept {
            assert(s.is(php::value::TYPE_STRING));
            return s.as<php::string>()->hashcode();
        }
    };
}

#endif // LIBPHPEXT_STRING_H
