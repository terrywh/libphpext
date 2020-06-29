#ifndef LIBPHPEXT_CALLABLE_H
#define LIBPHPEXT_CALLABLE_H

#include "vendor.h"
#include "type_code.h"

namespace php {
    template <bool RAU>
    class value_t;
    using value = value_t<true>;
    // 可调用（对象）
    class callable: public zval {
    public:
        // 可调用类型（虚拟类型）
        static constexpr type_code_t TYPE_CODE = FAKE_CALLABLE;
        // 构建
        callable(const ::php::value& cb);
        // 调用、执行（无参）
        ::php::value operator ()() const;
        // 调用、执行
        ::php::value operator ()(std::vector<::php::value> argv);
        // TODO
    };
}

#endif // LIBPHPEXT_CALLABLE_H
