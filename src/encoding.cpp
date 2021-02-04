#include "encoding.h"
#include "exception.h"
#include "cast.h"
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
        _convert_to_string(str);
        string& s = php::cast<php::string>(str);
        std::size_t l = php_url_decode(s.data(), s.size());
        s.shrink(l);
        return str;
    }
    //
    value bin2hex(std::string_view data) {
        php::value s = create_sstring(data.size() * 2);
        make_digest_ex(php::cast<string>(s).data(),
            reinterpret_cast<const unsigned char*>(data.data()), data.size());
        return s;
    }
    // 函数未导出，摘自 ext/standard/string.c
    static zend_string *php_hex2bin(const unsigned char *old, const size_t oldlen) {
        size_t target_length = oldlen >> 1;
        zend_string *str = zend_string_alloc(target_length, 0);
        unsigned char *ret = (unsigned char *)ZSTR_VAL(str);
        size_t i, j;

        for (i = j = 0; i < target_length; i++) {
            unsigned char c = old[j++];
            unsigned char l = c & ~0x20;
            int is_letter = ((unsigned int) ((l - 'A') ^ (l - 'F' - 1))) >> (8 * sizeof(unsigned int) - 1);
            unsigned char d;

            /* basically (c >= '0' && c <= '9') || (l >= 'A' && l <= 'F') */
            if (EXPECTED((((c ^ '0') - 10) >> (8 * sizeof(unsigned int) - 1)) | is_letter)) {
                d = (l - 0x10 - 0x27 * is_letter) << 4;
            } else {
                zend_string_efree(str);
                return NULL;
            }
            c = old[j++];
            l = c & ~0x20;
            is_letter = ((unsigned int) ((l - 'A') ^ (l - 'F' - 1))) >> (8 * sizeof(unsigned int) - 1);
            if (EXPECTED((((c ^ '0') - 10) >> (8 * sizeof(unsigned int) - 1)) | is_letter)) {
                d |= l - 0x10 - 0x27 * is_letter;
            } else {
                zend_string_efree(str);
                return NULL;
            }
            ret[i] = d;
        }
        ret[i] = '\0';
        return str;
    }
    // 
    value hex2bin(std::string_view str) {
        return value( php_hex2bin( reinterpret_cast<const unsigned char *>(str.data()), str.size()) );
    }
    //
    value json_encode(const value& v, int opts) {
        smart_str buffer {nullptr, 0};
        json_encode_to(&buffer, v, opts);
        return buffer.s != nullptr ? value(buffer.s) : value(nullptr);
    }
    void json_encode_to(smart_str* buffer, const value& v, int opts) {
        if(php_json_encode(buffer, v, opts) == FAILURE) cpp_rethrow();
    }
    // 
    value json_decode(std::string_view data) {
        value v;
        if(php_json_decode_ex(v, data.data(), data.size(),
            PHP_JSON_OBJECT_AS_ARRAY, PHP_JSON_PARSER_DEFAULT_DEPTH) == FAILURE) cpp_rethrow();
        return v;
    }
}