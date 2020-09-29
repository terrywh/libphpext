#include "callable.h"
#include "value.h"
#include "exception.h"

namespace php {
    // 构建：可调用函数
    callable::callable(const ::php::value& cb) {
        assert(zend_is_callable(cb, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr));
        ZVAL_COPY(this, cb);
    }
    // 销毁
    callable::~callable() {
        zval_ptr_dtor(this);
    }
    // 调用、执行（无参）
    ::php::value callable::operator ()() const {
        ::php::value rv;
        call_user_function(nullptr, nullptr, const_cast<callable*>(this), rv, 0, nullptr);
        // _call_user_function_ex(nullptr, const_cast<callable*>(this), rv, 0, nullptr);
        cpp_rethrow();
        return rv;
    }
    // 调用、执行
    ::php::value callable::operator ()(std::vector<::php::value> argv) {
        ::php::value rv;
        call_user_function(nullptr, nullptr, const_cast<callable*>(this), rv, 
            argv.size(), reinterpret_cast<zval*>(argv.data()));
        // _call_user_function_ex(nullptr, const_cast<callable*>(this), rv,
        //     argv.size(), reinterpret_cast<zval*>(argv.data()));
        cpp_rethrow();
        return rv;
    }
    // TODO
}
