#include "callback.h"
#include "module_entry.h"
#include "value.h"
#include "parameter.h"

namespace php {
    // 注册
    void callback::do_register(module_entry& module) {
        module.declare<callback>("_ext_callback_", ZEND_ACC_FINAL) // 禁止继承
            .declare<&callback::__construct>(env::key(method_name::__CONSTRUCTOR), {}, {})
            .declare<&callback::__invoke>(env::key(method_name::__INVOKE), {
                    {"argv", false, true, true}
            }, {TYPE_MIXED});
    }
    // 设置回调
    void callback::fn(std::function<php::value (php::parameters& params)> cb) {
        fn_ = new std::function<php::value (php::parameters& params)>( std::move(cb) );
    }
    // 构造（禁用）
    php::value callback::__construct(php::parameters &params) {
        throw php::error("Instantiation of 'Closure' is not allowed", -1);
    }
    // 实际执行过程
    php::value callback::__invoke(php::parameters &params) {
        php::value rv = fn_->operator()(params);
        delete fn_;
        fn_ = nullptr;
        return rv;
    }
}