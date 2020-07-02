#ifndef LIBPHPEXT_CALLBACK_H
#define LIBPHPEXT_CALLBACK_H

#include "vendor.h"

namespace php {
    class module_entry;
    class value;
    class parameters;
    // 包裹 C++ 回调
    class callback {
    public:
        // 注册
        static void do_register(module_entry& module);
        // 设置回调
        void fn(std::function<php::value (php::parameters& params)> cb);
        // 构造函数（禁用）
        php::value __construct(php::parameters& params);
        // 实际执行过程
        php::value __invoke(php::parameters& params);
    private:
        // 存储回调 (需要使用指针类型，保持 standard_layout 形式)
        std::function<php::value (php::parameters& params)>* fn_;
    };
}

#endif // LIBPHPEXT_CALLBACK_H
