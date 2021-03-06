#ifndef LIBPHPEXT_UTIL_H
#define LIBPHPEXT_UTIL_H

#include "vendor.h"
#include "value.h"

namespace php {
    // 
    const char* version();
    // 解析 form-data 数据
    value& parse_form_data(std::string_view str, value& array);
    // 解析 form-data 数据
    value parse_form_data(std::string_view str);
    // 生成 form-data 数据
    value build_form_data(const value& hash);
    // URL 解析
    std::shared_ptr<php_url> parse_url(std::string_view url);
    // 进程标题
    void process_title(std::string_view name);
    // 内存用量
    inline std::size_t memory_usage(bool r = false) {
        return zend_memory_usage(r); // r == true 时包含已分配但还未使用的部分
    }
    // 当前用户
    inline std::string_view current_user() {
        return {php_get_current_user(), static_cast<std::size_t>(SG(request_info).current_user_length)};
    }
    // 计算 T/t G/g M/m K/k 单位
    std::int64_t str2bytes(std::string_view str);
}

#endif // LIBPHPEXT_UTIL_H
