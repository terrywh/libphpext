#ifndef LIBPHPEXT_CLASS_BASIC_H
#define LIBPHPEXT_CLASS_BASIC_H

#include "value.h"
#include "class_entry.h"
#include "property_value.h"

namespace php {
    // PHP 类基类（可选，方便访问非同步属性等）
    template <class T>
    class class_basic {
    protected:
        inline object* self() const {
            return class_entry<T>::native( static_cast<const T*>(this) );
        }
        // 属性（可设置）
        inline property_value prop(const char* name) {
            return self()->prop(name);
        }
        // 属性（可设置）
        inline property_value prop(std::string_view name) {
            return self()->prop(name);
        }
        // 属性（可设置）
        inline property_value prop(const value& name) {
            return self()->prop(name);
        }
        // 属性设置
        inline void prop(const char* name, const value& data) {
            return prop(std::string_view(name), data);
        }
        // 属性设置
        inline void prop(std::string_view name, const value& data) {
            self()->prop(name, data);
        }
        // 属性设置
        inline void prop(const value& name, const value& data) {
            self()->prop(name, data);
        }
    };
}

#endif // LIBPHPEXT_CLASS_BASIC_H
