#ifndef LIBPHPEXT_OBJECT_H
#define LIBPHPEXT_OBJECT_H

#include "vendor.h"
#include "type_code.h"

namespace php {
    template <bool RAU>
    class value_t;
    using value = value_t<true>;
    class property;
    // 对象，对应 zend_object
    class object: public zend_object {
    public:
        // 对象类型
        static constexpr type_code_t TYPE_CODE = TYPE_OBJECT;
        // 创建制定名称的对象实例，并调用其 PHP 构造函数 (无参)
        static value create(std::string_view name);
        // 创建指定名称的对象实例，并调用其 PHP 构造函数
        static value create(std::string_view name, std::vector<value> argv);
        // 创建指定类型的对象实例，并调用其 PHP 构造函数 (无参)
        static value create(zend_class_entry* ce);
        // 创建指定类型的对象实例，并调用其 PHP 构造函数
        static value create(zend_class_entry* ce, std::vector<value> argv);
        // 调用成员函数（无参）
        static value call(object* obj, const value& name);
        // 调用成员函数
        static value call(object* obj, const value& name, const std::vector<value>& argv);
        // 调用成员函数
        value call(const value& name);
        // 调用成员函数
        value call(const value& name, const std::vector<value>& argv);
        // 属性（可设置）
        property prop(const char* name);
        // 属性（可设置）
        property prop(std::string_view name);
        // 属性（可设置）
        property prop(const value& name);
        // 属性设置
        void prop(const char* name, const value& data) {
            return prop(std::string_view(name), data);
        }
        // 属性设置
        void prop(std::string_view name, const value& data);
        // 属性设置
        void prop(const value& name, const value& data);

    };
}

#endif // LIBPHPEXT_OBJECT_H
