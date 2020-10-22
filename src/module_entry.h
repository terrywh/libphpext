#ifndef LIBPHPEXT_MODULE_ENTRY_H
#define LIBPHPEXT_MODULE_ENTRY_H

#include "vendor.h"
#include "hook_entry.h"
#include "dependency_entry.h"
#include "constant_entry.h"
#include "ini_entry.h"
#include "function_entry.h"
#include "class_entry.h"

namespace php {
    class module_entry;
    using info = std::pair<std::string, std::string>;
    using require = dependency_entry;
    using ini = ini_entry;
    using constant = constant_entry;
    // 模块描述
    class module_entry {
    private:
        // 当前实例
        static module_entry* self_;
        // 模块
        zend_module_entry module_;
        std::string                           name_; // 模块名称
        std::string                        version_; // 模块版本
        
        std::vector<info>  info_; // 模块信息
        std::vector<dependency_entry>  dependency_; // 模块依赖
        std::vector<constant_entry>      constant_; // 常量
        std::vector<ini_entry>                ini_; // 配置
        
        std::vector<zend_function_entry> function_; // 函数
        std::vector<class_entry_basic*>     class_; // 类表: 不销毁、不释放（内存由 PHP 管理）

        std::vector<hook_handler> mstartup_;
        std::vector<hook_handler> mshutdown_;
        std::vector<hook_handler> rstartup_;
        std::vector<hook_handler> rshutdown_;
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
        module_entry& operator -(info desc) {
            info_.push_back(std::move(desc));
            return *this;
        }
        // 为模块添加依赖
        module_entry& operator -(require r) {
            dependency_.push_back(std::move(r));
            return *this;
        }
        // 设置 ini 项目
        module_entry& operator -(ini i) {
            ini_.push_back(std::move(i));
            return *this;
        }
        // 定义扩展常量
        module_entry& operator -(constant c) {
            constant_.push_back(std::move(c));
            return *this;
        }
        // 函数
        // 使用 function<>(...) 生成
        module_entry& operator -(function_entry f) {
            function_.push_back(f);
            return *this;
        }
        // 周期
        module_entry& operator -(hook_entry&& h) {
            if(dynamic_cast<module_startup*>(&h)) 
                mstartup_.push_back(std::move(h.cb_));
            else if(dynamic_cast<request_startup*>(&h)) 
                rstartup_.push_back(std::move(h.cb_));
            else if(dynamic_cast<request_shutdown*>(&h)) 
                rshutdown_.push_back(std::move(h.cb_));
            else if(dynamic_cast<module_shutdown*>(&h)) 
                mshutdown_.push_back(std::move(h.cb_));
            else 
                throw php::type_error("unknown type of hook handler");
            return *this;
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
        // PHP 回调：请求终止
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
