#include "encoding.h"
#include <ext/standard/url.h>
#include <ext/standard/base64.h>
#include <ext/standard/md5.h>

namespace php {
    // BASE64
    value base64_encode(std::string_view data) {
        return php_base64_encode(
            reinterpret_cast<const unsigned char*>(data.data()), data.size());
    }
    // BASE64
    value base64_decode(std::string_view data) {
        return php_base64_decode(
            reinterpret_cast<const unsigned char*>(data.data()), data.size());
    }
    // URL
    value url_encode(std::string_view data) {
        return php_url_encode(data.data(), data.size());
    }
    // URL (inplace)
    value& url_decode(value& str) {
        string* s = str.as<string>();
        std::size_t l = php_url_decode(s->val, s->len);
        s->shrink(l);
        return str;
    }
    //
    value bin2hex(std::string_view data) {
        php::value s = string::create(data.size() * 2);
        make_digest_ex(s.as<string>()->val,
            reinterpret_cast<const unsigned char*>(data.data()), data.size());
        return s;
    }
    //
    value json_encode(const value& v, int opts) {
        smart_str buffer {nullptr, 0};
        json_encode_to(&buffer, v, opts);
        return buffer.s != nullptr ? value(buffer.s) : value(nullptr);
    }
    // 
    value json_decode(std::string_view data) {
        value v;
        if(php_json_decode_ex(v, data.data(), data.size(),
            PHP_JSON_OBJECT_AS_ARRAY, PHP_JSON_PARSER_DEFAULT_DEPTH) == FAILURE) try_rethrow();
        return v;
    }
}