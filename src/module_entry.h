#ifndef LIBPHPEXT_MODULE_ENTRY_H
#define LIBPHPEXT_MODULE_ENTRY_H

#include "vendor.h"
#include "dependence.h"
#include "constant_entry.h"
#include "ini_entry.h"
#include "function_entry.h"
#include "class_entry.h"

namespace php {
    class module_entry;
    // 模块启动处理程序
    struct module_startup {
        module_startup(const std::function<bool (module_entry&)>& h): handler(h) {}
        std::function<bool (module_entry&)> handler;
    };
    // 请求启动处理程序（对应不同模块生命周期阶段）
    struct request_startup {
        request_startup(const std::function<void (module_entry&)>& h): handler(h) {}
        std::function<void (module_entry&)> handler;
    };
    // 请求终止处理程序（对应不同模块生命周期阶段）
    struct request_shutdown {
        request_shutdown(const std::function<void (module_entry&)>& h): handler(h) {}
        std::function<void (module_entry&)> handler;
    };
    // 模块终止处理程序（对应不同模块生命周期阶段）
    struct module_shutdown {
        module_shutdown(const std::function<void (module_entry&)>& h): handler(h) {}
        std::function<void (module_entry&)> handler;
    };
    
    // 模块描述
    class module_entry {
    private:
        // 当前实例
        static module_entry* self_;
        // 模块
        zend_module_entry module_;
        std::string                           name_; // 模块名称
        std::string                        version_; // 模块版本
        using description_type = std::pair<std::string, std::string>;
        std::vector<description_type>  description_; // 模块信息
        dependences                     dependence_; // 模块依赖
        std::vector<constant_entry>       constant_; // 常量
        ini_entries                            ini_; //
        function_entries                  function_; // 函数表
        std::vector<class_entry_basic*>      class_; // 类表: 不销毁、不释放（内存由 PHP 管理）

        std::vector<module_startup>     module_startup_handler_; // 模块启动回调
        std::vector<request_startup>   request_startup_handler_; // 请求启动回调
        std::vector<request_shutdown> request_shutdown_handler_; // 请求停止回调
        std::vector<module_shutdown>   module_shutdown_handler_; // 模块停止回调
        // 正向调用回调
        // 正向调用（调用失败时中断，并返回 false 否则返回 true）
        template <class Handlers>
        void invoke_fwd(Handlers& handlers_) {
            for(auto i=handlers_.begin(); i!= handlers_.end(); ++i) {
                i->handler(*module_entry::self());
            }
        }
        bool invoke_fwd(std::vector<module_startup>& handlers_) {
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
        module_entry& setup(std::string_view name, std::string_view data) {
            zend_string* zn = zend_string_init_interned(name.data(), name.size(), true);
            ini_ += ini_entry {zn, data};
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
            zend_string* zn = zend_string_init_interned(name.data(), name.size(), true);
            constant_.push_back({zn, data});
            return *this;
        }
        // 函数（注意，由于实际指针数据等由对应对象持有，需要原始指针地址）
        template <value fn(parameters& params)>
        module_entry& declare(std::string_view name, std::initializer_list<argument_info> pi, return_info&& ri) {
            zend_string* zn = zend_string_init_interned(name.data(), name.size(), true);
            function_.append({ function_entry::function<fn>, zn, std::move(ri), std::move(pi) });
            return *this;
        }
        // 函数
        template <value fn(parameters& params)>
        module_entry& declare(std::string_view name, std::initializer_list<argument_info> pi) {
            return declare<fn>(name, std::move(pi), return_info());
        }
        // 函数
        template <value fn(parameters& params)>
        module_entry& declare(std::string_view name, return_info&& ri) {
            return declare<fn>(name, {}, std::move(ri));
        }
        // 函数
        template <value fn(parameters& params)>
        module_entry& declare(std::string_view name) {
            return declare<fn>(name, {}, return_info());
        }
        // 类
        template <class T>
        class_entry<T>& declare(std::string_view name, std::uint32_t flag = 0) {
            zend_string* zn = zend_string_init_interned(name.data(), name.size(), true);
            // 统一使用 PHP 内存管理
            class_entry<T>* x = reinterpret_cast<class_entry<T>*>(pemalloc(sizeof(class_entry<T>), true));
            new (x) class_entry<T>(zn, flag);
            class_.emplace_back(x); // 多态形式，将父类指针放入容器
            return *x;
        }
        // 实际模块地址
        operator zend_module_entry*();
    private:
        // PHP 回调：模块启动
        static zend_result on_module_startup  (int type, int module);
        // PHP 回调：请求启动
        static zend_result on_request_startup (int type, int module);
        // PHP 回调：请求终止
        static zend_result on_request_shutdown(int type, int module);
        // PHP 回调：模块终止
        static zend_result on_module_shutdown (int type, int module);
        // PHP 回调：模块信息
        static void on_module_info(zend_module_entry *zend_module);
    };

}

#endif // LIBPHPEXT_MODULE_ENTRY_H
