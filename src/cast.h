#ifndef LIBPHPEXT_CAST_H
#define LIBPHPEXT_CAST_H

#include "vendor.h"
#include "type.h"
#include "class_entry.h"
#include "value.h"
#include "array.h"
#include "string.h"
#include "object.h"

namespace php {
    // 创建指定长度的字符串
    value create_sstring(std::size_t size = 0, bool persist = false);
    // 创建字符串（复制）
    value create_pstring(std::string_view str);
    // 创建制定名称的对象实例（未调用 __construct 函数）
    value create_object(std::string_view name);
    // 创建指定类型的对象实例
    template <class T>
    value create_object() {
        return value(*class_entry<T>::entry());
    }
    //
    value create_callable(std::string_view name);
    //
    value create_callable(value obj, std::string_view name);
    //
    value create_callable(std::function<value (parameters& params)> cb);
    // 创建数组
    value create_array(std::size_t size = 0);
    // 获取对应 CPP 实例指针
    template <class T>
    inline T& cast(value& v) {
        assert(zval_get_type(v) == TYPE_OBJECT && Z_OBJCE_P( static_cast<zval*>(v) ) == *class_entry<T>::entry());
        return *class_entry<T>::native(Z_OBJ_P(static_cast<zval*>(v)));
    }
    // ARRAY
    template <>
    inline array& cast(value& v) {
        assert(zval_get_type(v) == TYPE_ARRAY);
        return *reinterpret_cast<array*>(Z_PTR_P( static_cast<zval*>(v) ));
    }
    // STRING
    template <>
    inline string& cast(value& v) {
        assert(zval_get_type(v) == TYPE_STRING);
        return *reinterpret_cast<string*>(Z_PTR_P( static_cast<zval*>(v) ));
    }
    // OBJECT
    template <>
    inline object& cast(value& v) {
        assert(zval_get_type(v) == TYPE_OBJECT);
        return *reinterpret_cast<object*>(Z_PTR_P( static_cast<zval*>(v) ));
    }
}

#endif // LIBPHPEXT_CAST_H