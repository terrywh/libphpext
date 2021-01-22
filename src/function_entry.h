#ifndef LIBPHPEXT_FUNCTION_ENTRY_H
#define LIBPHPEXT_FUNCTION_ENTRY_H

#include "vendor.h"
#include "type.h"
#include "value.h"
#include "parameter.h"
#include "argument_entry.h"
#include "exception.h"

namespace php {
    // 函数项
    class function_entry {
    public:
        // 保留参数描述引用数据后构建 entry
        operator zend_function_entry() const;
        // 构建 zend_function_entry 列表
        static zend_function_entry* finalize(std::vector<zend_function_entry>& entry);
    protected:
        zif_handler                         handler_; // 目标函数
        zend_string*                        name_; // 函数名
        std::vector<zend_internal_arg_info> argv_; // 描述信息（返回、参数等）
        uint32_t                            flag_; // 标志位
        // 
        function_entry(zif_handler fn, zend_string* name, 
            std::vector<type_desc>&& desc, uint32_t flag);
        // 构建函数项
        function_entry(zif_handler fn, std::string_view name, 
            std::vector<type_desc>&& desc, uint32_t flag)
        : function_entry(fn, zend_string_init_interned(name.data(), name.size(), true), std::move(desc), flag) {}
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
            catch (const std::exception& e) {
                php_rethrow(e);
            }
            // 非预期范围的异常继续在 C++ 侧抛出
        }
        // 普通方法
        template <value fn(parameters& params)>
        friend function_entry function(std::string_view name, std::initializer_list<type_desc> desc);
        // 静态方法（类）
        template <value fn(parameters& params)>
        friend function_entry static_method(std::string_view name, std::initializer_list<type_desc> desc);
        template <auto>
        friend class method;

        template <auto>
        friend class private_method;

        template <auto>
        friend class protected_method;
    };
    // 模块函数（注意，由于实际指针数据等由对应对象持有，需要原始指针地址）
    template <value fn(parameters& params)>
    function_entry function(std::string_view name, std::initializer_list<type_desc> desc) {
        return { function_entry::function<fn>, name, std::move(desc), 0 };
    }
    // 模块函数
    template <value fn(parameters& params)>
    function_entry function(std::string_view name) {
        return function<fn>(name, {});
    }
    // 静态方法（注意，由于实际指针数据等由对应对象持有，需要原始指针地址）
    template <value fn(parameters& params)>
    function_entry static_method(std::string_view name, std::initializer_list<type_desc> desc) {
        return { function_entry::function<fn>, name, std::move(desc), ZEND_ACC_PUBLIC | ZEND_ACC_STATIC };
    }
    // 静态方法
    template <value fn(parameters& params)>
    function_entry static_method(std::string_view name) {
        return static_method<fn>(name, {});
    }
}

#endif // LIBPHPEXT_FUNCTION_ENTRY_H
