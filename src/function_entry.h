#ifndef LIBPHPEXT_FUNCTION_ENTRY_H
#define LIBPHPEXT_FUNCTION_ENTRY_H

#include "vendor.h"
#include "value.h"
#include "parameter.h"
#include "argument_info.h"
#include "exception.h"

namespace php {
    // 函数项
    class function_entry {
    public:
        // 构建函数项
        function_entry(zif_handler fn, zend_string* name, return_info&& ri,
            std::initializer_list<argument_info>&& pi);
        // 构建 zend_function_entry 用于注册函数
        operator zend_function_entry() const;
        // PHP 回调
        template <value fn(parameters& params)>
        static void callback(zend_execute_data* execute_data, zval* return_value) {
            parameters params(execute_data);
            // TODO class return_type
            value& rv = *reinterpret_cast<value*>(return_value);
            try {
                if(execute_data->func->common.required_num_args > ZEND_NUM_ARGS())
                    throw exception("Expects at least "
                        + std::to_string(execute_data->func->common.required_num_args) + " parameter(s), "
                        + std::to_string(ZEND_NUM_ARGS()) + " given");

                rv = fn(params);
            }
            catch (const throwable& e) {
                rethrow(e);
                return;
            }
            // 非预期范围的异常继续在 C++ 侧抛出
        }
    private:
        // 目标函数
        zif_handler                         fn_;
        // 函数名
        zend_string*                        nm_;
        // 描述信息（返回、参数等）
        std::vector<zend_internal_arg_info> ai_;

        friend class function_entries;
    };

    // 函数表
    class function_entries {
    private:
        // 保持引用（以使对应内存地址有效）
        std::vector<function_entry>         refs_;
        std::vector<zend_function_entry> entries_;
        
    public:
        // 加入函数描述
        function_entries& operator +=(function_entry&& desc) {
            refs_.emplace_back(std::move(desc));
            return *this;
        }
        // 构建 zend_function_entry 列表
        operator zend_function_entry*();
    };
}

#endif // LIBPHPEXT_FUNCTION_ENTRY_H
