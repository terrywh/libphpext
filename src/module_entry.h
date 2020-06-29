#ifndef LIBPHPEXT_module_entry_H
#define LIBPHPEXT_module_entry_H

#include "vendor.h"
#include "dependence.h"
#include "constant_entry.h"
#include "ini_entry.h"
#include "function_entry.h"
#include "class_entry.h"

namespace php {
    class module_entry;
    // 回调函数
    using startup_handler = std::function<bool (module_entry&)>;
    using shutdown_handler = std::function<void (module_entry&)>;
    // 模块启动处理程序
    struct module_startup {
        module_startup(const startup_handler& h): handler(h) {}
        startup_handler handler;
    };
    // 请求启动处理程序（对应不同模块生命周期阶段）
    struct request_startup {
        request_startup(const startup_handler& h): handler(h) {}
        startup_handler handler;
    };
    // 请求终止处理程序（对应不同模块生命周期阶段）
    struct request_shutdown {
        request_shutdown(const shutdown_handler& h): handler(h) {}
        shutdown_handler handler;
    };
    // 模块终止处理程序（对应不同模块生命周期阶段）
    struct module_shutdown {
        module_shutdown(const shutdown_handler& h): handler(h) {}
        shutdown_handler handler;
    };
    
    // 模块描述
    class module_entry {
    private:
        // 当前实例
        static module_entry* self_;
        // 模块名称
        std::string name_;
        // 模块版本
        std::string version_;
        // 模块信息
        using description_type = std::pair<std::string, std::string>;
        std::vector<description_type>  description_;
        // 模块依赖
        dependences dependence_;
        std::vector<constant_entry> constant_entry_;
        ini_entries ini_entry_;
        // 模块
        zend_module_entry module_;
        // 函数表
        function_entries function_entry_;
        // 模块启动回调
        std::vector<module_startup>  module_startup_handler_;
        // 请求启动回调
        std::vector<request_startup>  request_startup_handler_;
        // 请求停止回调
        std::vector<request_shutdown> request_shutdown_handler_;
        // 模块停止回调
        std::vector<module_shutdown> module_shutdown_handler_;
        // 正向调用回调
        // 正向调用（调用失败时中断，并返回 false 否则返回 true）
        template <class Handlers>
        bool invoke_fwd(Handlers& handlers_) {
            for(auto i=handlers_.begin(); i!= handlers_.end(); ++i) {
                if(! i->handler(*module_entry::self()) ) return false;
            }
            return true;
        }
        // 反向调用
        template <class Handlers>
        void invoke_bwd(Handlers& handlers_) {
            for(auto i=handlers_.rbegin(); i!= handlers_.rend(); ++i) {
                i->handler(*module_entry::self());
            }
        }
    public:
        // 模块描述实例
        inline static module_entry* self(module_entry* e = nullptr) {
            return self_;
        }
        // 模块编号
        int module;
        // 模块名称版本构造
        module_entry(std::string_view name, std::string_view version);
        // 描述模块（在 php_info() / 模块信息中展示)
        module_entry& describe(std::string_view name, std::string_view info) {
            description_.emplace_back(name, info);
            return *this;
        }
        // 设置 ini 项目
        module_entry& setup(std::string_view name, std::string_view data, refer* name_ref = nullptr) {
            zend_string* zn = zend_string_init_interned(name.data(), name.size(), true);
            if(name_ref) *name_ref = zn;
            ini_entry_ += ini_entry {zn, data};
            return *this;
        }
        // 为模块添加依赖
        module_entry& require(const char* module_name, const char* version) {
            dependence_ += dependence {module_name, "ge", version, MODULE_DEP_REQUIRED};
            return *this;
        }
        // 为模块添加依赖
        module_entry& require(const char* module_name) {
            dependence_ += dependence {module_name, nullptr, nullptr, MODULE_DEP_REQUIRED};
            return *this;
        }
        // 添加模块启动处理程序
        module_entry& on(const module_startup& handler) {
            module_startup_handler_.push_back(handler);
            return *this;
        }
        // 添加请求启动处理程序
        module_entry& on(const request_startup& handler) {
            request_startup_handler_.push_back(handler);
            return *this;
        }
        // 添加请求终止处理程序
        module_entry& on(const request_shutdown& handler) {
            request_shutdown_handler_.push_back(handler);
            return *this;
        }
        // 添加模块终止处理程序
        module_entry& on(const module_shutdown& handler) {
            module_shutdown_handler_.push_back(handler);
            return *this;
        }
        // 定义扩展常量
        module_entry& define(std::string_view name, const php::value& data) {
            constant_entry_.push_back({name, data});
            return *this;
        }
        // 函数（注意，由于实际指针数据等由对应对象持有，需要原始指针地址）
        template <value fn(parameters& params)>
        module_entry& declare(std::string_view name, std::initializer_list<argument_info> pi,
                return_info&& ri, refer* name_ref = nullptr) {
            zend_string* zn = zend_string_init_interned(name.data(), name.size(), true);
            if(name_ref) *name_ref = zn; // 函数名称字符串的引用
            function_entry_ += function_entry(function_entry::callback<fn>, zn, std::move(ri), std::move(pi));
            return *this;
        }
        // 函数
        template <value fn(parameters& params)>
        module_entry& declare(std::string_view name, std::initializer_list<argument_info> pi, refer* name_ref = nullptr) {
            return declare<fn>(name, std::move(pi), return_info(), name_ref);
        }
        // 函数
        template <value fn(parameters& params)>
        module_entry& declare(std::string_view name, refer* name_ref = nullptr) {
            return declare<fn>(name, {}, return_info(), name_ref);
        }
        template <class T>
        class_entry<T>& declare(std::string_view name) {

        }
        // 实际模块地址
        operator zend_module_entry*();
    private:
        // PHP 回调：模块启动
        static int on_module_startup  (int type, int module);
        // PHP 回调：请求启动
        static int on_request_startup (int type, int module);
        // PHP 回调：请求终止
        static int on_request_shutdown(int type, int module);
        // PHP 回调：模块终止
        static int on_module_shutdown (int type, int module);
        // PHP 回调：模块信息
        static void on_module_info(zend_module_entry *zend_module);
    };

}

#endif // LIBPHPEXT_module_entry_H
