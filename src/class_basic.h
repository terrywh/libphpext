#ifndef LIBPHPEXT_CLASS_BASIC_H
#define LIBPHPEXT_CLASS_BASIC_H

#include "value.h"
#include "class_entry.h"

namespace php {
    // PHP 类基类（可选）
    template <class T>
    class class_basic {
    protected:
        inline object* self() const {
            return class_entry<T>::native( static_cast<const T*>(this) );
        }
    };
}

#endif // LIBPHPEXT_CLASS_BASIC_H
