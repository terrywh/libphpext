#include "util.h"
#include "value.h"
#include "callable.h"

namespace php {
    // 解析 form-data 数据
    value& parse_str_to(std::string_view str, value& a) {
        if(!a.is(TYPE_ARRAY)) zend_try_array_init(a);
        // 参考 string.c:parse_str 实现
        char * r = estrndup(str.data(), str.size());
        sapi_module.treat_data(PARSE_STRING, r, a);
        return a;
    }
    // 解析 form-data 数据
    value parse_str(std::string_view str) {
        php::value a = array::create(4);
        return parse_str_to(str, a);
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
