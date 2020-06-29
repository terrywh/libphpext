#ifndef LIBPHPEXT_INI_ENTRY_H
#define LIBPHPEXT_INI_ENTRY_H

#include "vendor.h"

namespace php {
    // 定义 INI 配置项
    class ini_entry: public zend_ini_entry_def {
    public:
        ini_entry()
        : zend_ini_entry_def {nullptr} {}
        // 构建项
        ini_entry(zend_string* key, std::string_view val);
        // 移动
        ini_entry(ini_entry&& e);
        // 释放
        ~ini_entry();
    private:
        // 修改回调
        static int on_modify(zend_ini_entry* entry, zend_string* nv, void* a1, void* a2, void* a3, int stage);
    };

    class ini_entries {
    private:
        std::vector<ini_entry> entry_;
    public:
        ini_entries& operator += (ini_entry&& e) {
            entry_.emplace_back(std::move(e));
            return *this;
        }
        // 输出定义
        operator zend_ini_entry_def*();
    };
}

#endif // LIBPHPEXT_INI_ENTRY_H