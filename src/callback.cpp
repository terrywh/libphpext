#include "callback.h"
#include "environ.h"
#include "module_entry.h"
#include "value.h"
#include "parameter.h"
#include "exception.h"
#include "function_entry_method.h"

namespace php {
    // 注册
    void callback::declare(module_entry& module) {
        auto& x = module.declare<callback>("__callback__", ZEND_ACC_FINAL) // 禁止继承
            - method<&callback::__invoke>(environ::key(method_name::__INVOKE), {
                    FAKE_MIXED, // return
                    FAKE_MIXED | ALLOW_NULL | VARIADIC,
                })
            - private_method<&callback::__construct>(environ::key(method_name::__CONSTRUCTOR)) // 禁止构造
            ;
    }
    // 设置回调
    void callback::fn(std::function<php::value (php::parameters& params)> cb) {
        fn_ = reinterpret_cast<callback_type*>(pemalloc(sizeof(cb), false));
        new (fn_) callback_type( std::move(cb) );
    }
    // 构造（禁用）
    php::value callback::__construct(php::parameters &params) {
        throw php::type_error("Instantiation of '__callback__' is not allowed");
    }
    // 实际执行过程
    php::value callback::__invoke(php::parameters &params) {
        assert(fn_ != nullptr);
        php::value rv = fn_->operator()(params);
        delete fn_;
        fn_ = nullptr;
        return rv;
    }
}