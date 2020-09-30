#include "module_entry.h"
#include "env.h"
#include "callback.h"

namespace php {
    // 当前实例
    module_entry* module_entry::self_;
    // 模块名称版本构造
    module_entry::module_entry(std::string_view name, std::string_view version)
    : module_ {
        STANDARD_MODULE_HEADER_EX,
        nullptr, // 配置 ini entry 此项数据由 zend engine 填充
        nullptr, // 依赖模块，暂不设置 --> (1)
        nullptr, // 模块名称
        nullptr, // 函数表，暂不设置 --> (1)
        on_module_startup,
        on_module_shutdown,
        on_request_startup,
        on_request_shutdown,
        on_module_info,
        nullptr, // 版本号
        STANDARD_MODULE_PROPERTIES,
    }
    , name_(name)
    , version_(version)
    , module(0) {
        // 仅允许单例使用
        self_ = this;
        module_.name = name_.c_str();
        module_.version = version_.c_str();
        // 基础依赖 (内部使用了对应的部分功能)
        require("standard");
        require("json");
        require("date");
    }
    // 实际模块地址
    module_entry::operator zend_module_entry*() {
        static bool repeat_guard = false;
        // 禁止重复使用
        assert(!repeat_guard);
        repeat_guard = true;
        // (1) 延迟设定依赖及函数表
        module_.deps                  = dependence_;
        module_.functions             = function_;

        return &module_;
    }
    // PHP 回调：模块启动
    zend_result module_entry::on_module_startup  (int type, int module) {
        self()->module = module;
        env::init(); // 环境相关初始化
        // 配置 ini 项
        zend_register_ini_entries(self()->ini_, module);
        // 注册 常量
        for(auto& c : self()->constant_)  zend_register_constant(&c);
        // 注册 内部 类
        callback::do_register(*self());
        // 注册 类
        for(auto& c : self()->class_) c->do_register(module);
        //
        if(!self()->invoke_fwd(self()->module_startup_handler_))
            return FAILURE;
        return SUCCESS;
    }
    // PHP 回调：请求启动
    zend_result module_entry::on_request_startup (int type, int module) {
        if(!self()->invoke_fwd(self()->request_startup_handler_)) 
            return FAILURE;
        return SUCCESS;
    }
    // PHP 回调：请求终止
    zend_result module_entry::on_request_shutdown(int type, int module) {
        self()->invoke_bwd(self()->request_shutdown_handler_);
        return SUCCESS;
    }
    // PHP 回调：模块终止
    zend_result module_entry::on_module_shutdown (int type, int module) {
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