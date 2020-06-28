#ifndef LIBPHPEXT_ARRAY_H
#define LIBPHPEXT_ARRAY_H

#include "vendor.h"
#include "type_code.h"

namespace php {
    template <bool RAU>
    class value_t;
    using value = value_t<true>;
    // 数组
    class array: public zend_array {
    public:
        static type_code_t TYPE_CODE;
        // 数组元素访问
        value& operator [](int idx) const;
        // 数组元素访问
        value& operator [](const value& idx) const;
        // TODO
    };

}

#endif // LIBPHPEXT_ARRAY_H
