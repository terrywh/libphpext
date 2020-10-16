#ifndef LIBPHPEXT_ENCODING_H
#define LIBPHPEXT_ENCODING_H

#include "vendor.h"
#include "exception.h"
#include "value.h"
#include "string.h"

namespace php {
    // BASE64
    value base64_encode(std::string_view data);
    // BASE64
    value base64_decode(std::string_view data);
    // URL
    value url_encode(std::string_view data);
    // URL (inplace)
    value& url_decode(value& str);
    // URL
    inline value url_decode(std::string_view data) {
        php::value v {data};
        return url_decode(v);
    }
    // 
    value bin2hex(std::string_view data);
    //
    inline value hex2bin(std::string_view data) {
        // 由于 PHP 未导出内部编码函数，此处相当于用户调用
        // 若考虑效率问题，可考虑复制 ext/standard/string.c:php_hex2bin() 函数相关代码
        return callable("hex2bin")({data});
    }
    // JSON 编码至指定容器（可使用 string_buffuer / string_builder 作为 buffer 使用）
    inline void json_encode_to(smart_str* buffer, const value& v, int opts = PHP_JSON_UNESCAPED_UNICODE) {
        if(php_json_encode(buffer, v, opts) == FAILURE) cpp_rethrow();
    }
    //
    value json_encode(const value& v, int opts = PHP_JSON_UNESCAPED_UNICODE);
    // 
    value json_decode(std::string_view data);
}

#endif // LIBPHPEXT_ENCODING_H