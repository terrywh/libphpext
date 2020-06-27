#include "string.h"
#include "value.h"

namespace php {
    // PHP 数据序列化（JSON）
    string_builder& operator << (string_builder& sb, const value& data) {
        if(!data.is(value::TYPE_STRING))
            php_json_encode(&sb.str_, data, PHP_JSON_UNESCAPED_UNICODE);
        else
            sb.append(static_cast<std::string_view>(data));
		return sb;
    }
    // 对应类型 参见 value::as<string>()
    value::types string::TYPE = value::TYPE_STRING;
    // 创建指定长度的字符串
    string* string::create(std::size_t size, bool persist) {
        return reinterpret_cast<string*>(zend_string_alloc(size, persist));
    }
    // 创建字符串（复制）
    string* string::create(std::string_view s, bool persist) {
        return reinterpret_cast<string*>(zend_string_init(s.data(), s.size(), persist));
    }
}
