#include "object.h"
#include "value.h"
#include "exception.h"
#include "property_value.h"

namespace php {
    // 调用成员函数（无参）
    value object::call(zend_object* obj, const zend_string* name) {
        value rv;
        // zend_execute_API.c: _call_user_function_ex
        zend_fcall_info fci;
        fci.size = sizeof(fci);
        fci.object = obj;
        ZVAL_STR(&fci.function_name, const_cast<zend_string*>(name));
        // ZVAL_COPY_VALUE(&fci.function_name, name);
        fci.retval = rv;
        fci.param_count = 0;
        fci.params = nullptr;
        zend_call_function(&fci, NULL);

        cpp_rethrow();
        return rv;
    }
    // 调用成员函数
    value object::call(zend_object* obj, const zend_string* name, std::uint32_t argc, zval* argv) {
        value rv;
        // zend_execute_API.c: _call_user_function_ex
        zend_fcall_info fci;
        fci.size = sizeof(fci);
        fci.object = obj;
        ZVAL_STR(&fci.function_name, const_cast<zend_string*>(name));
        // ZVAL_COPY_VALUE(&fci.function_name, name);
        fci.retval = rv;
        fci.param_count = argc;
        fci.params = argv;
        zend_call_function(&fci, NULL);

        cpp_rethrow();
        return rv;
    }
    // 调用成员函数
    value object::call(const value& name) {
        return call(this, name);
    }
    // 调用成员函数（注意调用参数要求独立构建的 value / zval 容器）
    value object::call(const value& name, std::vector<value> argv) {
        return call(this, name, argv.size(), reinterpret_cast<zval *>(argv.data()));
    }
    // 属性（可设置）
    property_value object::prop(const char* name) {
        return prop(std::string_view(name));
    }
    // 属性（可设置）
    property_value object::prop(std::string_view name) {
        return property_value(this,
                // 内部字符串实际会按哈系查询复用
                zend_string_init_interned(name.data(), name.size(), true));
    }
    // 属性（可设置）
    property_value object::prop(const value& name) {
        assert(name.is(TYPE_STRING));
        return property_value(this, name);
    }
    // 属性设置
    void object::prop(std::string_view name, const value& data) {
        zend_class_entry *ce = EG(fake_scope);
        EG(fake_scope) = this->ce;
        this->handlers->write_property(this, // 内部已进行引用计数
            zend_string_init_interned(name.data(), name.size(), true), data, nullptr);
        EG(fake_scope) = ce;
    }
    // 属性设置
    void object::prop(const value& name, const value& data) {
        assert(name.is(TYPE_STRING));
        zend_class_entry *ce = EG(fake_scope);
        EG(fake_scope) = this->ce;
        // 内部已进行引用计数
        this->handlers->write_property(this, name, data, nullptr);
        EG(fake_scope) = ce;
    }

}