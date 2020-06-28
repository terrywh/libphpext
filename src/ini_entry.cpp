#include "ini_entry.h"

namespace php {
    // 构建 ini 定义
    ini_entry::ini_entry(std::string_view key, std::string_view val)
    : zend_ini_entry_def {
        nullptr, // name
        nullptr, // ini_entry::on_modify,
        nullptr, // arg1
        nullptr, // arg2
        nullptr, // arg3
        nullptr, // value
        nullptr, // displayer
        static_cast<std::uint32_t>(val.size()), // value_length
        static_cast<std::uint16_t>(key.size()), // name_length
        ZEND_INI_ALL }// modifiable
    {
        // -> (X)
        char* buffer = new char[key.size() + val.size() + 2];
        std::memcpy(buffer, key.data(), key.size());
        buffer[key.size()] = '\0';
        std::memcpy(buffer + key.size() + 1, val.data(), val.size());
        buffer[key.size() + key.size() + 1] = '\0';

        name    = buffer;
        value   = buffer + key.size() + 1;
    }
    // 移动
    ini_entry::ini_entry(ini_entry&& e) {
        std::memcpy(this, &e, sizeof(ini_entry));
        e.name = nullptr; // -> (X)
    }
    // 释放
    ini_entry::~ini_entry() {
        // (X) ->
        char* buffer = const_cast<char*>(name);
        if(buffer) delete[] buffer;
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