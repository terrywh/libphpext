#include "string_builder.h"
#include "value.h"

namespace php {
    // PHP 数据序列化（JSON）
    string_builder& operator << (string_builder& sb, const value& data) {
        if(!data.is(TYPE_STRING))
            php_json_encode(&sb.str_, data, PHP_JSON_UNESCAPED_UNICODE);
        else
            sb.append(static_cast<std::string_view>(data));
		return sb;
    }
    // 获取构建的字符串
    value string_builder::str() {
        assert(str_.s); // 保证字符串存在
        smart_str_0(&str_); // 保证 \0 结束
        zend_string* s = str_.s;
        // 当前 buffer 需要清空
        memset(&str_, 0, sizeof(smart_str));
        return value(s, false);
    }
}