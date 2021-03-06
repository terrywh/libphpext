#ifndef LIBPHPEXT_OBJECT_H
#define LIBPHPEXT_OBJECT_H

#include "vendor.h"
#include "type.h"

namespace php {
    template <class T>
    class class_entry;
    class value;
    class property_value;
    // 对象，对应 zend_object
    class object: public zend_object {
    public:
        // 对象类型
        static constexpr type_code TYPE_CODE = TYPE_OBJECT;
        // 调用成员函数（无参）
        static value call(zend_object* obj, const zend_string* name);
        // 调用成员函数
        static value call(zend_object* obj, const zend_string*, std::uint32_t argc, zval* argv);
        // 调用成员函数
        value call(const value& name);
        // 调用成员函数（注意调用参数要求独立构建的 value / zval 容器）
        value call(const value& name, std::vector<value> argv);
        // 属性（可设置）
        property_value prop(const char* name);
        // 属性（可设置）
        property_value prop(std::string_view name);
        // 属性（可设置）
        property_value prop(const value& name);
        // 属性设置
        void prop(const char* name, const value& data) {
            return prop(std::string_view(name), data);
        }
        // 属性设置
        void prop(std::string_view name, const value& data);
        // 属性设置
        void prop(const value& name, const value& data);
        // 
        std::int64_t handle_id() {
            // Z_OBJ_HANDLE_P(zv)
            return this->handle;
        }
    };
}

#endif // LIBPHPEXT_OBJECT_H
