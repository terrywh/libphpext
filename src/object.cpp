#include "object.h"
#include "value.h"
#include "env.h"
#include "exception.h"
#include "property.h"

namespace php {
    // 创建制定名称的对象实例，并调用其 PHP 构造函数 (无参)
    zend_object* object::create(std::string_view name) {
        // ??? 类名是否应该使用内部持久型字符串
        // zend_string* str = zend_string_init_interned(name.data(), name.size(), 1);
        zend_string* str = zend_string_init(name.data(), name.size(), 1);
        zend_class_entry* ce = zend_lookup_class(str);
        return create(ce);
    }
    // 创建指定类型的对象实例，并调用其 PHP 构造函数 (无参)
    zend_object* object::create(zend_class_entry* ce) {
        value obj;
        object_init_ex(obj, ce);
        return obj;
    }
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
        fci.no_separation = (zend_bool) 1;
        zend_call_function(&fci, NULL);

        try_rethrow();
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
        fci.no_separation = (zend_bool) 1;
        zend_call_function(&fci, NULL);

        try_rethrow();
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
    property object::prop(const char* name) {
        return prop(std::string_view(name));
    }
    // 属性（可设置）
    property object::prop(std::string_view name) {
        return property(this,
                // 内部字符串实际会按哈系查询复用
                zend_string_init_interned(name.data(), name.size(), true));
    }
    // 属性（可设置）
    property object::prop(const value& name) {
        assert(name.is(TYPE_STRING));
        return property(this, name);
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