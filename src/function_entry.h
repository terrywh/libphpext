#ifndef LIBPHPEXT_FUNCTION_ENTRY_H
#define LIBPHPEXT_FUNCTION_ENTRY_H

#include "vendor.h"
#include "value.h"
#include "parameter.h"
#include "argument_entry.h"
#include "exception.h"

namespace php {
    // 函数项
    class function_entry {
    public:
        // function_entry(function_entry&& entry)
        // : handler_(entry.handler_)
        // , name_(entry.name_)
        // , argv_(std::move(entry.argv_))
        // , flag_(entry.flag_) {
        //     entry.handler_ = nullptr;
        //     entry.name_ = nullptr;
        // }

        // 保留参数描述引用数据后构建 entry
        operator zend_function_entry() const;
        // 构建 zend_function_entry 列表
        static zend_function_entry* finalize(std::vector<zend_function_entry>& entry);
       
    protected:
        zif_handler                         handler_; // 目标函数
        zend_string*                        name_; // 函数名
        std::vector<zend_internal_arg_info> argv_; // 描述信息（返回、参数等）
        uint32_t                            flag_; // 标志位

        function_entry(zif_handler fn, zend_string* name, 
            std::initializer_list<argument_entry>&& pi, return_entry&& ri, uint32_t flag);
        // 构建函数项
        function_entry(zif_handler fn, std::string_view name, 
            std::initializer_list<argument_entry>&& pi, return_entry&& ri, uint32_t flag)
        : function_entry(fn, zend_string_init_interned(name.data(), name.size(), true), std::move(pi), std::move(ri), flag) {}
        // PHP 回调
        template <value fn(parameters& params)>
        static void function(zend_execute_data* execute_data, zval* return_value) {
            parameters params(execute_data);
            value& rv = *reinterpret_cast<value*>(return_value);
            try {
                if(execute_data->func->common.required_num_args > ZEND_NUM_ARGS())
                    throw argument_count_error("Expects at least "
                        + std::to_string(execute_data->func->common.required_num_args) + " parameter(s), "
                        + std::to_string(ZEND_NUM_ARGS()) + " given");

                rv = fn(params);
            }
            catch (const throwable& e) {
                php_rethrow(e);
            }
            // 非预期范围的异常继续在 C++ 侧抛出
        }
        template <value fn(parameters& params)>
        friend function_entry function(std::string_view name, std::initializer_list<argument_entry> pi, return_entry&& ri);

        template <auto>
        friend class method;

        template <auto>
        friend class private_method;

        template <auto>
        friend class protected_method;

        template <value fn(parameters& params)>
        friend function_entry static_method(std::string_view name, std::initializer_list<argument_entry> pi, return_entry&& ri);
    };

    // 模块函数（注意，由于实际指针数据等由对应对象持有，需要原始指针地址）
    template <value fn(parameters& params)>
    function_entry function(std::string_view name, std::initializer_list<argument_entry> pi, return_entry&& ri) {
        return { function_entry::function<fn>, name, std::move(pi), std::move(ri), 0 };
    }
    // 模块函数
    template <value fn(parameters& params)>
    function_entry function(std::string_view name, std::initializer_list<argument_entry> pi) {
        return function<fn>(name, std::move(pi), {});
    }
    // 模块函数
    template <value fn(parameters& params)>
    function_entry function(std::string_view name, return_entry&& ri) {
        return function<fn>(name, {}, std::move(ri));
    }
    // 模块函数
    template <value fn(parameters& params)>
    function_entry function(std::string_view name) {
        return function<fn>(name, {}, return_entry());
    }

    // 静态方法（注意，由于实际指针数据等由对应对象持有，需要原始指针地址）
    template <value fn(parameters& params)>
    function_entry static_method(std::string_view name, std::initializer_list<argument_entry> pi, return_entry&& ri) {
        return { function_entry::function<fn>, name, std::move(pi), std::move(ri), ZEND_ACC_PUBLIC | ZEND_ACC_STATIC };
    }
    // 静态方法
    template <value fn(parameters& params)>
    function_entry static_method(std::string_view name, std::initializer_list<argument_entry> pi) {
        return static_method<fn>(name, std::move(pi), return_entry());
    }
    // 静态方法
    template <value fn(parameters& params)>
    function_entry static_method(std::string_view name, return_entry&& ri) {
        return static_method<fn>(name, {}, std::move(ri));
    }
    // 静态方法
    template <value fn(parameters& params)>
    function_entry static_method(std::string_view name) {
        return static_method<fn>(name, {}, {});
    }
}

#endif // LIBPHPEXT_FUNCTION_ENTRY_H
