#include "object.h"
#include "constant.h"
#include "exception.h"

namespace php {
    // 对应类型 参见 value::as<object>()
    type_code_t object::TYPE_CODE = TYPE_OBJECT;
    // 调用成员函数（无参）
    value object::call(object* obj, const value& name) {
        value rv;
        // zend_execute_API.c: _call_user_function_ex
        zend_fcall_info fci;
        fci.size = sizeof(fci);
        fci.object = obj;
        ZVAL_COPY_VALUE(&fci.function_name, name);
        fci.retval = rv;
        fci.param_count = 0;
        fci.params = nullptr;
        fci.no_separation = (zend_bool) 1;
        zend_call_function(&fci, NULL);

        try_rethrow();
        return rv;
    }
    // 调用成员函数
    value object::call(object* obj, const value& name, const std::vector<value>& argv) {
        // 参数引用源数据变量，不进行相关的释放
        std::vector<value_t<false>> params(argv.begin(), argv.end());
        value rv;
        // zend_execute_API.c: _call_user_function_ex
        zend_fcall_info fci;
        fci.size = sizeof(fci);
        fci.object = obj;
        ZVAL_COPY_VALUE(&fci.function_name, name);
        fci.retval = rv;
        fci.param_count = params.size();
        fci.params = reinterpret_cast<zval*>(params.data());
        fci.no_separation = (zend_bool) 1;
        zend_call_function(&fci, NULL);

        try_rethrow();
        return rv;
    }
    // 调用成员函数
    value object::call(const value& name) {
        return call(this, name);
    }
    // 调用成员函数
    value object::call(const value& name, const std::vector<value>& argv) {
        return call(this, name, argv);
    }
    // 创建制定名称的对象实例，并调用其 PHP 构造函数 (无参)
    value object::create(std::string_view name) {
        // ??? 类名是否应该使用内部持久型字符串
        // zend_string* str = zend_string_init_interned(name.data(), name.size(), 1);
        zend_string* str = zend_string_init(name.data(), name.size(), 1);
        zend_class_entry* ce = zend_lookup_class(str);
        return create(ce);
    }
    // 创建指定名称的对象实例，并调用其 PHP 构造函数
    value object::create(std::string_view name, std::vector<value> argv) {
        // ??? 类名是否应该使用内部持久型字符串
        // zend_string* str = zend_string_init_interned(name.data(), name.size(), 1);
        zend_string* str = zend_string_init(name.data(), name.size(), 1);
        zend_class_entry* ce = zend_lookup_class(str);
        return create(ce);
    }
    // 创建指定类型的对象实例，并调用其 PHP 构造函数 (无参)
    value object::create(zend_class_entry* ce) {
        value_t<false> obj;
        object_init_ex(obj, ce);
        object::call(obj.as<object>(), constant::string(constant::method_name::__CONSTRUCTOR));
        return obj;
    }
    // 创建指定类型的对象实例，并调用其 PHP 构造函数
    value object::create(zend_class_entry* ce, std::vector<value> argv) {
        value_t<false> obj;
        object_init_ex(obj, ce);
        object::call(obj.as<object>(), constant::string(constant::method_name::__CONSTRUCTOR), std::move(argv));
        return obj;
    }
}