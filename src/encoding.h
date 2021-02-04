#ifndef LIBPHPEXT_ENCODING_H
#define LIBPHPEXT_ENCODING_H

#include "vendor.h"
#include "value.h"

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
    value hex2bin(std::string_view data);
    // JSON 编码至指定容器（可使用 string_buffuer / string_builder 作为 buffer 使用）
    void json_encode_to(smart_str* buffer, const value& v, int opts = PHP_JSON_UNESCAPED_UNICODE);
    //
    value json_encode(const value& v, int opts = PHP_JSON_UNESCAPED_UNICODE);
    // 
    value json_decode(std::string_view data);
}

#endif // LIBPHPEXT_ENCODING_H