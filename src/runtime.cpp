#include "runtime.h"
#include "value.h"
#include "conversion.h"
#include "array.h"
#include "array_iterator.h"
#include <sstream>

PHPAPI extern char *php_ini_opened_path;

namespace php {
    // 未定义引用
    value runtime::undefined_value;

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
        current_working_directory = getcwd(nullptr, 0);
        // 方法1：(不含 PHP 内置参数)
        // php::value& argv = *reinterpret_cast<php::value*>(
        //     zend_hash_find_ex_ind(&EG(symbol_table), ZSTR_KNOWN(ZEND_STR_ARGV), 1));
        // ss << argv;
        // 方法2：(不含 PHP 内置参数)
        // for(int i=1;i<SG(request_info).argc;++i)
        //     ss << " " << SG(request_info).argv[i];
        // 方法3：(可获得所有参数)
        // 参见：php_cli.c 文件 912 / 1286 行
        int first = 0;
        while(std::strcmp(sapi_module.executable_location, SG(request_info).argv[first]) != 0) --first;
        argv_.resize(SG(request_info).argc - first);
        memcpy(argv_.data(), &SG(request_info).argv[first], argv_.size() * sizeof(char*));
    }
}