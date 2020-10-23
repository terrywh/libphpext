#include "util.h"
#include "value.h"
#include "callable.h"

namespace php {
    const char* version() {
        return LIBPHPEXT_VERSION;
    }
    // 解析 form-data 数据
    value& parse_form_data(std::string_view str, value& a) {
        if(!a.is(TYPE_ARRAY)) zend_try_array_init(a);
        // 参考 string.c:parse_str 实现
        char * r = estrndup(str.data(), str.size());
        sapi_module.treat_data(PARSE_STRING, r, a);
        return a;
    }
    // 解析 form-data 数据
    value parse_form_data(std::string_view str) {
        php::value a = array::create(4);
        return parse_form_data(str, a);
    }
    // 生成 form-data 数据
    value build_form_data(const value& hash) {
        if(!hash.is(TYPE_ARRAY)) return zend_empty_string;
        smart_str str {nullptr, 0};
        php_url_encode_hash_ex(hash, &str,
            /* num_prefix */ nullptr, /* num_prefix_len */ 0,
            /* key_prefix */ nullptr, /* key_prefix_len */ 0,
            /* key_suffix */ nullptr, /* key_suffix_len */ 0,
            /* type */ nullptr, "&", PHP_QUERY_RFC3986);
        // PHP_QUERY_RFC1738 SPACE => +
        // PHP_QUERY_RFC3986 SPACE => %20
        return php::value(str.s, false);
    }

    // URL 解析
    std::shared_ptr<php_url> parse_url(std::string_view url) {
        return std::shared_ptr<php_url>(
            php_url_parse_ex(url.data(), url.size()), php_url_free);
    }
    // 设置进程标题
    void process_title(std::string_view name) {
        // PHP 未导出相关设置函数
        callable("cli_set_process_title")({name});
    }
}
