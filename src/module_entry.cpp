#include "module_entry.h"
#include "environ.h"
#include "runtime.h"
#include "class_closure.h"

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
        module_.deps      = dependency_entry::finalize(dependency_);
        module_.functions = function_entry::finalize(function_);

        return &module_;
    }
    // PHP 回调：模块启动
    zend_result module_entry::on_module_startup  (int type, int module) {
        self()->module = module;
        environ::init(); // 环境相关初始化
        
        ini_entry::do_register(self()->ini_, module); // 配置 ini 项
        constant_entry::do_register(self()->constant_, module); // 注册 常量

        class_closure::declare(*self()); // 声明 内部 类
        class_entry_basic::do_register(self()->class_, module); // 注册 类
        
        try { hook_entry::emit(self()->mstartup_, *self()); }
        catch(const std::exception& ex) { return FAILURE; }
        return SUCCESS;
    }
    zend_result module_entry::on_request_startup(int type, int module) {
        runtime::init(); // 运行时数据初始化

        try { hook_entry::emit(self()->rstartup_, *self()); }
        catch(const std::exception& ex) { return FAILURE; }
        return SUCCESS;
    }
    // PHP 回调：请求终止
    zend_result module_entry::on_request_shutdown(int type, int module) {
        try { hook_entry::fire(self()->rshutdown_, *self()); }
        catch(const std::exception& ex) { return FAILURE; }
        return SUCCESS;
    }
    // PHP 回调：模块终止
    zend_result module_entry::on_module_shutdown (int type, int module) {
        try { hook_entry::fire(self()->mshutdown_, *self()); }
        catch(const std::exception& ex) { return FAILURE; }
        return SUCCESS;
    }
    // PHP 回调：模块信息
    void module_entry::on_module_info(zend_module_entry *module) {
        php_info_print_table_start();
        php_info_print_table_row(2, "version", self()->version_.c_str());
        for(auto& i : self()->info_) {
            php_info_print_table_row(2, i.first.c_str(), i.second.c_str());
        }
        php_info_print_table_end();
        display_ini_entries(module);
    }
}