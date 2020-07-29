#include "callable.h"
#include "value.h"
#include "exception.h"

namespace php {
    // 构建：可调用对象
    callable::callable(const ::php::value& cb) {
        assert(zend_is_callable(this, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr));
        ZVAL_COPY(this, cb);
    }
    // 调用、执行（无参）
    ::php::value callable::operator ()() const {
        ::php::value rv;
        _call_user_function_ex(nullptr, const_cast<callable*>(this), rv, 0, nullptr);
        try_rethrow();
        return rv;
    }
    // 调用、执行
    ::php::value callable::operator ()(std::vector<::php::value> argv) {
        ::php::value rv;
        _call_user_function_ex(nullptr, const_cast<callable*>(this), rv,
            argv.size(), reinterpret_cast<zval*>(argv.data()));
        try_rethrow();
        return rv;
    }
    // TODO
}
