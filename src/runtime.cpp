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
    static std::string cmd_;
    // 文本常量：命令行
    std::string_view runtime::cmd() {
        return cmd_;
    }
    // 变量获取
    value& runtime::g(std::string_view name) {
        return php::array::find(&EG(symbol_table), php::string::create(name, false));
    }
    // 常量获取
    value& runtime::c(std::string_view name) {
        zend_string* zn = zend_string_init_interned(name.data(), name.size(), true);
        return *reinterpret_cast<value*>(zend_get_constant(zn));
    }
    // 环境初始化
    void runtime::init() {
        current_working_directory = getcwd(nullptr, 0);

        std::ostringstream ss;
        ss << runtime::path(path::PHP_BINARY_FILE);
        for(int i=1;i<SG(request_info).argc;++i)
        // auto& args = reinterpret_cast<value*>(&PG(http_globals)[TRACK_VARS_SERVER])->as<php::array>();
        // ss << " -c " << php::env::path(php::path::PHP_LOADED_INI_FILE);
        // for (auto arg : args)
            ss << " " << SG(request_info).argv[i];
        cmd_ = ss.str();
    }
}