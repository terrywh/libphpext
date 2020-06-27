#ifndef LIBPHPEXT_OBJECT_H
#define LIBPHPEXT_OBJECT_H

#include "forward.h"
#include "value.h"
#include "constants.h"

namespace php {
    void rethrow();
    // 对象，对应 zend_object
    class object: public zend_object {
    public:
        // 对应类型 参见 value::as<object>()
        static value::types TYPE;
        // 调用成员函数（无参）
        static value call(object* obj, const value& name);
        // 调用成员函数
        static value call(object* obj, const value& name, const std::vector<value>& argv);
        // 调用成员函数
        inline value call(const value& name) { return call(this, name); }
        // 调用成员函数
        inline value call(const value& name, const std::vector<value>& argv) { return call(this, name, argv); }
        // 创建制定名称的对象实例，并调用其 PHP 构造函数 (无参)
        static object* create(std::string_view name);
        // 创建指定名称的对象实例，并调用其 PHP 构造函数
        static object* create(std::string_view name, std::vector<value> argv);
        // 创建指定类型的对象实例，并调用其 PHP 构造函数 (无参)
        static object* create(zend_class_entry* ce);
        // 创建指定类型的对象实例，并调用其 PHP 构造函数
        static object* create(zend_class_entry* ce, std::vector<value> argv);
    };
}

#endif // LIBPHPEXT_OBJECT_H
