#include <main/php.h>
#include <ext/standard/info.h>
// #include <stdio.h>

#ifndef zend_result
#define zend_result int
#endif

zend_result on_module_startup  (int type, int module) {
    // printf("%016lx %s\n", environ,  getenv("BYTED_HOST_IPV6"));
    return SUCCESS;
}

zend_result on_module_shutdown (int type, int module) {
    // printf("%016lx %s\n", environ,  getenv("BYTED_HOST_IPV6"));
    return SUCCESS;
}

void on_module_info(zend_module_entry *zend_module) {
    php_print_info(PHP_INFO_ENVIRONMENT);
}
// PHP 扩展模块入口
ZEND_DLEXPORT zend_module_entry* get_module() {
    static zend_module_entry module = {
        STANDARD_MODULE_HEADER_EX,
        NULL, // 配置 ini entry 此项数据由 zend engine 填充
        NULL, // 依赖模块，暂不设置 --> (1)
        "example3", // 模块名称
        NULL, // 函数表，暂不设置 --> (1)
        on_module_startup, // on_module_startup,
        on_module_shutdown, // on_module_shutdown,
        NULL, // on_request_startup,
        NULL, // on_request_shutdown,
        on_module_info, // on_module_info,
        "1.0.0", // 版本号
        STANDARD_MODULE_PROPERTIES,
    };
    // printf("%016lx %016lx %s\n", &environ, environ, getenv("BYTED_HOST_IPV6"));
    return &module;
}
