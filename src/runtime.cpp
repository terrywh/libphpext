#include "runtime.h"
#include "value.h"
#include "conversion.h"
#include "array.h"
#include "array_iterator.h"

PHPAPI extern char *php_ini_opened_path;

namespace php {
    // 未定义引用
    value runtime::undefined_value;
    // null 引用
    value runtime::null_value;

    static char* current_working_directory;
    // 文本常量：路径
    std::string_view runtime::path(php::path pn) {
        switch(pn) {
        case php::path::CURRENT_WORKING_DIRECTORY:
            return current_working_directory;
        case php::path::PHP_BINARY_FILE:
            return PG(php_binary);
        case php::path::PHP_LOADED_INI_FILE:
            return php_ini_opened_path;
        default:
            return {};
        }
    }
    static std::vector<const char*> argv_;
    // 文本常量：命令行
    std::vector<const char*>& runtime::argv() {
        return argv_;
    }
    // 变量获取
    value& runtime::g(std::string_view name) {
        zval* v = zend_hash_str_find(&EG(symbol_table), name.data(), name.size());
        return v ? *reinterpret_cast<value*>(v) : runtime::undefined_value;
    }
    // 常量获取
    value& runtime::c(std::string_view name) {
        zend_string* zn = zend_string_init_interned(name.data(), name.size(), true);
        return *reinterpret_cast<value*>(zend_get_constant(zn));
    }
    // 环境初始化
    void runtime::init() {
        null_value = nullptr;
        // 工作路径
        current_working_directory = getcwd(nullptr, 0);
        // 可执行文件
        argv_.push_back( PG(php_binary) );
        // 参数信息，默认状态不包含 php 内置处理的参数，这里须额外补充 ini 设置
        argv_.push_back("-c");
        argv_.push_back(php_ini_opened_path);
        // 不含 PHP 内置参数
        for(int i=0;i<SG(request_info).argc;++i) {
            argv_.push_back(SG(request_info).argv[i]);
        }
    }
}