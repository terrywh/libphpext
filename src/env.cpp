#include "env.h"
#include "value.h"
#include "conversion.h"

namespace php {
    // 未定义引用
    value env::undefined_value;
    // 文本常量：方法名称
    static const char* method_name_cstr[] = {
        ZEND_CLONE_FUNC_NAME,
        ZEND_CONSTRUCTOR_FUNC_NAME,
        ZEND_DESTRUCTOR_FUNC_NAME,
        ZEND_INVOKE_FUNC_NAME,
        ZEND_GET_FUNC_NAME,
        ZEND_SET_FUNC_NAME,
        ZEND_UNSET_FUNC_NAME,
        ZEND_ISSET_FUNC_NAME,
        ZEND_CALL_FUNC_NAME,
        ZEND_CALLSTATIC_FUNC_NAME,
        ZEND_TOSTRING_FUNC_NAME,
        ZEND_INVOKE_FUNC_NAME,
        ZEND_DEBUGINFO_FUNC_NAME,
    };
    // 文本数据：方法名称
    static zend_string* method_name_zstr[static_cast<int>(method_name::METHOD_NAME_MAX)];
    // 文本常量：方法名称
    value env::key(method_name mn) {
        return method_name_zstr[ static_cast<int>(mn) ];
    }
    // 文本常量：类名（属性名、方法名、类名等，也可考虑在声明时设置引用获取）
    value env::key(std::string_view name) {
        return zend_string_init_interned(name.data(), name.size(), true);
    }
    // 待读取 ini 项
    env::ini::ini(std::string_view name)
    : entry_(reinterpret_cast<zend_ini_entry*>(
        zend_hash_str_find_ptr(EG(ini_directives), name.data(), name.size())
    )) {}

    env::ini::operator std::int64_t() const {
        return entry_ ? zend_strtod(ZSTR_VAL(entry_->value), nullptr) : 0;
    }
    env::ini::operator std::size_t() const {
        return entry_ ? std::strtoul(ZSTR_VAL(entry_->value), nullptr, 10) : 0;
    }
    env::ini::operator value() const {
        return entry_ ? value(entry_->value) : value(nullptr);
    }
    env::ini::operator std::string() const {
        return entry_ ? std::string(ZSTR_VAL(entry_->value), ZSTR_LEN(entry_->value)) : std::string();
    }
    std::int64_t env::ini::bytes() const {
        return entry_? to_bytes({ZSTR_VAL(entry_->value), ZSTR_LEN(entry_->value)}) : 0;
    }
    // 环境初始化
    void env::init() {
        for(int i = 0; i < static_cast<int>(method_name::METHOD_NAME_MAX); ++i) {
            method_name_zstr[i] = zend_string_init_interned(method_name_cstr[i], std::strlen(method_name_cstr[i]), true);
        }
    }
}