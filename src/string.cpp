#include "string.h"
#include "value.h"

namespace php {
    // 字符串类型
    type_code_t string::TYPE_CODE = TYPE_STRING;
    string_builder string::build() {
        return string_builder();
    }
    // PHP 数据序列化（JSON）
    string_builder& operator << (string_builder& sb, const value& data) {
        if(!data.is(TYPE_STRING))
            php_json_encode(&sb.str_, data, PHP_JSON_UNESCAPED_UNICODE);
        else
            sb.append(static_cast<std::string_view>(data));
		return sb;
    }
    // 创建指定长度的字符串
    value string::create(std::size_t size, bool persist) {
        // 新创建（不用增加引用计数）
        return value(zend_string_alloc(size, persist), false); 
    }
    // 创建字符串（复制）
    value string::create(std::string_view s, bool persist) {
        // 新创建（不用增加引用计数）
        return value(zend_string_init(s.data(), s.size(), persist), false);
    }
    
}
