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
        callback():fn_(nullptr) {}
        // 声明 callback 类型
        static void declare(module_entry& module);
        using callback_type = std::function<php::value (php::parameters& params)>;
        // 设置回调
        void fn(callback_type cb);
        // 构造函数（禁用）
        php::value __construct(php::parameters& params);
        // 实际执行过程
        php::value __invoke(php::parameters& params);
    private:
        // 存储回调 (需要使用指针类型，保持 standard_layout 形式)
        callback_type* fn_;
    };
}

#endif // LIBPHPEXT_CALLBACK_H
