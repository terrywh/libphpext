#include "class_closure.h"
#include "environ.h"
#include "module_entry.h"
#include "value.h"
#include "parameter.h"
#include "exception.h"
#include "method_entry.h"

namespace php {
    // 注册
    void class_closure::declare(module_entry& module) {
        auto& x = module.declare<class_closure>("__closure__", ZEND_ACC_FINAL) // 禁止继承
            - method<&class_closure::__invoke>(environ::key(method_name::__INVOKE), {
                    {FAKE_MIXED}, // return
                    {FAKE_MIXED | VARIADIC, "any"},
                })
            - private_method<&class_closure::__construct>(environ::key(method_name::__CONSTRUCTOR)) // 禁止构造
            ;
    }
    // 设置回调
    void class_closure::set(std::function<php::value (php::parameters& params)> cb) {
        fn_ = reinterpret_cast<callback_type*>(pemalloc(sizeof(cb), false));
        new (fn_) callback_type( std::move(cb) );
    }
    // 构造（禁用）
    php::value class_closure::__construct(php::parameters &params) {
        throw php::type_error("Instantiation of '__callback__' is not allowed");
    }
    // 实际执行过程
    php::value class_closure::__invoke(php::parameters &params) {
        assert(fn_ != nullptr);
        php::value rv = fn_->operator()(params);
        fn_->~callback_type();
        pefree(fn_, false);
        fn_ = nullptr;
        return rv;
    }
}