#include "ini_entry.h"

namespace php {
    // 构建 ini 定义
    ini_entry::ini_entry(std::string_view key, std::string_view val, modifiable mod)
    : field_(zend_string_init_interned(key.data(), key.size(), true))
    , value_(zend_string_init(val.data(), val.size(), false)) {


        // key->val, // name
        // nullptr, // ini_entry::on_modify,
        // nullptr, // arg1
        // nullptr, // arg2
        // nullptr, // arg3
        // nullptr, // value
        // nullptr, // displayer
        // static_cast<std::uint32_t>(val.size()), // value_length
        // static_cast<std::uint16_t>(key->len), // name_length
        // ZEND_INI_ALL }// modifiable
    }
    // 移动
    ini_entry::ini_entry(ini_entry&& e) {
        field_ = e.field_;
        value_ = e.value_;
        e.field_ = nullptr;
        e.value_ = nullptr;
    }
    // 释放
    ini_entry::~ini_entry() {
        if(field_) zend_string_free(field_);
        if(value_) zend_string_free(value_);
    }

    int ini_entry::on_modify(zend_ini_entry* entry, zend_string* nv, void* a1, void* a2, void* a3, int stage) {
        // TODO 实现 ini 的动态变更通知
        return SUCCESS;
    }
    // 输出定义
    void ini_entry::do_register(std::vector<ini_entry>& entry, int module) {
        std::vector<zend_ini_entry_def> f(entry.size() + 1);  // 补充结束标志
        for(int i=0;i<entry.size();++i) {
            f[i].name = entry[i].field_->val;
            f[i].name_length = entry[i].field_->len;
            f[i].value = entry[i].value_->val;
            f[i].value_length = entry[i].value_->len;
            f[i].modifiable = static_cast<uint8_t>(entry[i].modifiable_);
            // TODO on_modify
        }
        // Zend 引擎内部会对 name / value 数据进行数据复制
        zend_register_ini_entries(f.data(), module);
        entry.clear(); // 对 INI 数据而言，复制后源数据可以清理
    }
}