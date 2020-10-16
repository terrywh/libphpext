#ifndef LIBPHPEXT_INI_ENTRY_H
#define LIBPHPEXT_INI_ENTRY_H

#include "vendor.h"

namespace php {
    // 定义 INI 配置项
    class ini_entry {
    public:
        enum modifiable {
            ALL = ZEND_INI_ALL,
            USER = ZEND_INI_USER,
            PERDIR = ZEND_INI_PERDIR,
            SYSTEM = ZEND_INI_SYSTEM,
        };
        ini_entry() {}
        // 构建项
        ini_entry(std::string_view key, std::string_view val, modifiable = ALL);
        // 移动
        ini_entry(ini_entry&& e);
        // 释放
        ~ini_entry();
        // 输出定义
        static void do_register(std::vector<ini_entry>& entry, int module);
    private:
        zend_string* field_;
        zend_string* value_;
        modifiable   modifiable_;

        // 修改回调
        static int on_modify(zend_ini_entry* entry, zend_string* nv, void* a1, void* a2, void* a3, int stage);
    };
}

#endif // LIBPHPEXT_INI_ENTRY_H