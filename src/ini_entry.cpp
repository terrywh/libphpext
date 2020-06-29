#include "ini_entry.h"

namespace php {
    // 构建 ini 定义
    ini_entry::ini_entry(zend_string* key, std::string_view val)
    : zend_ini_entry_def {
        key->val, // name
        nullptr, // ini_entry::on_modify,
        nullptr, // arg1
        nullptr, // arg2
        nullptr, // arg3
        nullptr, // value
        nullptr, // displayer
        static_cast<std::uint32_t>(val.size()), // value_length
        static_cast<std::uint16_t>(key->len), // name_length
        ZEND_INI_ALL }// modifiable
    {
        // -> (X)
        value = strndup(val.data(), val.size());
    }
    // 移动
    ini_entry::ini_entry(ini_entry&& e) {
        assert(sizeof(ini_entry) == sizeof(zend_ini_entry_def));
        std::memcpy(this, &e, sizeof(ini_entry));
        e.value = nullptr; // -> (X)
    }
    // 释放
    ini_entry::~ini_entry() {
        // (X) ->
        char* buffer = const_cast<char*>(value);
        if(buffer)
            free(buffer);
    }

    int ini_entry::on_modify(zend_ini_entry* entry, zend_string* nv, void* a1, void* a2, void* a3, int stage) {
        // TODO 实现 ini 的动态变更通知
        return SUCCESS;
    }
    // 输出定义
    ini_entries::operator zend_ini_entry_def*() {
        if(entry_.empty() || entry_.back().name) { 
            entry_.emplace_back(); // 补充结束标志
        }
        return entry_.data();
    }
}