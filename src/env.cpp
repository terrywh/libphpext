#include "env.h"
#include "conversion.h"

namespace php {
    // 文本常量：方法名称
    static const char* method_name_cstr[] = {
        ZEND_CONSTRUCTOR_FUNC_NAME,
        ZEND_DESTRUCTOR_FUNC_NAME,
        ZEND_INVOKE_FUNC_NAME,
    };
    // 文本常量：方法名称
    value env::str(method_name mn) {
        static value str[static_cast<int>(method_name::METHOD_NAME_MAX)];
        int i = static_cast<int>(mn);
        // 若常量还未创建
        if(!str[i].is(TYPE_STRING)) {
            const char* s = method_name_cstr[i];
            // 创建内部字符串
            str[i] = zend_string_init_interned(s, std::strlen(s), true);
        }
        return str[i];
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
        return to_bytes({ZSTR_VAL(entry_->value), ZSTR_LEN(entry_->value)});
    }
}