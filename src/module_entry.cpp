#include "module_entry.h"

namespace php {
    // 模块名称版本构造
    module_entry::module_entry(std::string_view name, std::string_view version)
    : name_(name)
    , version_(version)
    , module(0) {
        module_ = {
            STANDARD_MODULE_HEADER_EX,
            nullptr, // 配置 ini entry 此项数据由 zend engine 填充
            nullptr, // 依赖模块，暂不设置 --> (1)
            name_.c_str(), // 模块名称
            nullptr, // 函数表，暂不设置 --> (1)
            on_module_startup,
            on_module_shutdown,
            on_request_startup,
            on_request_shutdown,
            on_module_info,
            version_.c_str(),
            STANDARD_MODULE_PROPERTIES,
        };
        // 仅允许单例使用
        assert(self() == nullptr);
        self(this);
        // 基础依赖 (内部使用了对应的部分功能)
        dependence_ += {"standard", "ge", "8.0.0", MODULE_DEP_REQUIRED};
        dependence_ += {"json",     "ge", "8.0.0", MODULE_DEP_REQUIRED};
        dependence_ += {"date",     "ge", "8.0.0", MODULE_DEP_REQUIRED};
    }
    // 实际模块地址
    module_entry::operator zend_module_entry*() {
        static bool repeat = false;
        // 禁止重复使用
        assert(!repeat);
        repeat = true;
        // (1) 延迟设定依赖及函数表
        module_.deps                  = dependence_;
        module_.functions             = function_entry_;

        return &module_;
    }
    // PHP 回调：模块启动
    int module_entry::on_module_startup  (int type, int module) {
        self()->module = module;
        for(auto& c :self()->constant_entry_) {
            zend_register_constant(&c);
        }
        if(!self()->invoke_fwd(self()->module_startup_handler_)) 
            return FAILURE;
        return SUCCESS;
    }
    // PHP 回调：请求启动
    int module_entry::on_request_startup (int type, int module) {
        if(!self()->invoke_fwd(self()->request_startup_handler_)) 
            return FAILURE;
        return SUCCESS;
    }
    // PHP 回调：请求终止
    int module_entry::on_request_shutdown(int type, int module) {
        self()->invoke_bwd(self()->request_shutdown_handler_);
        return SUCCESS;
    }
    // PHP 回调：模块终止
    int module_entry::on_module_shutdown (int type, int module) {
        self()->invoke_bwd(self()->module_shutdown_handler_);
        return SUCCESS;
    }
    // PHP 回调：模块信息
    void module_entry::on_module_info(zend_module_entry *zend_module) {
        php_info_print_table_start();
        php_info_print_table_row(2, "version", self()->version_.c_str());
        for(auto& i : self()->description_) {
            php_info_print_table_row(2, i.first.c_str(), i.second.c_str());
        }
        php_info_print_table_end();
    }
}