#include "../phpext.h"

// 所有导出到 PHP 的函数必须符合下面形式：
php::value hello(php::parameters& params) {
    php::process_title("hello world");
    std::string user { php::current_user() };
    return "hello: " + user + ", from: " + php::runtime::argv()[0]; // 常用内部类型双向转换
}
// 引用型参数可改变其值
php::value plus_5(php::parameters& params) {
    php::value  x = params[0]; // 复制
    php::value& y = params[0]; // 引用
    
    x += 5; // 本地复制
    y += 5; // 修改参数
    // 也可直接对原始参数赋值
    // params[0] += 5;
    return x;
}
// 调用对象函数
php::value call_method(php::parameters& params) {
    return php::cast<php::object>(params[0]).call("format", {"Y-m-d H:i:s"});
}
// 放入 C++ 容器
php::value container(php::parameters& params) {
    std::map<php::value, php::value> c1;
    std::unordered_map<php::value, php::value> c2;

    c1[params[0]] = params[1]; // Sortable
    c2[params[0]] = params[1]; // Hashable

    return std::int64_t(c1.size() + c2.size());
}
// 调用回调
php::value invoke(php::parameters& params) {
    return php::value(params[0]({"world"}));
}
// 遍历数组
php::value walk(php::parameters& params) {
    auto& array = php::cast<php::array>(params[0]);
    int count = array.size();
    // >= C++11
    // for(auto x : array) {
    //     std::cout << x.first << " => " << x.second << std::endl;
    //     ++count;
    // }
    for(auto i = array.begin(); i!= array.end(); ++i) {
        std::cout << i->first << " => " << i->second << std::endl;
        ++count;
    }
    // for(auto i = array.rbegin(); i!= array.rend(); ++i) {
    //     std::cout << i->first << " => " << i->second << std::endl;
    //     ++count;
    // }
    return count;
}
// 读取配置
php::value conf_bytes(php::parameters& params) {
    return php::environ::ini(params[0]).byte_size();
}
// 访问属性
php::value property(php::parameters& params) {
    // 假设 d 原始值为 1
    auto d = php::cast<php::object>(params[0]).prop("d");
    // 函数设置：不会出发上述 d 值刷新
    php::cast<php::object>(params[0]).prop("d", 3);
    // 原地设置 -> 1 + 1 = 2
    d += 1;
    std::cout << d << " days\n";
    return d;
}
using php::module_startup;
using php::request_startup;
using php::request_shutdown;
using php::module_shutdown;
using php::info;
using php::require;
using php::ini;
using php::constant;
using php::function;

extern "C" {
    // PHP 扩展模块入口
    ZEND_DLEXPORT zend_module_entry* get_module() {
        // 构建模块：在离开函数后保留 module 定义
        static php::module_entry module("example1", "1.0.0");
        // 模块周期
        module
            - module_startup([] (php::module_entry& module) {
                std::cout << "example1 --> module started" << std::endl;
            })
            - request_startup([] (php::module_entry& module) {
                std::cout << "example1 --> request started" << std::endl;
            })
            - request_shutdown([] (php::module_entry& module) {
                std::cout << "example1 --> request shutdown" << std::endl;
            })
            - module_shutdown([] (php::module_entry& module) {
                std::cout << "example1 --> module shutdown" << std::endl;
            });
        // 模块定义
        module
            - require("pcre", "8.0.0") // 依赖
            - require("date")
            - info("info1", "content 1") // 说明信息
            - info("info2", "content 2")
            - info("hello", "world")
            - ini("example.hello", "value") // 配置
            - ini("example.size", "2M")
            - constant("CPP_CONSTANT_1", "abcdef") // 常量
            - constant("CPP_CONSTANT_2", 123456);
        // 函数声明
        module
            - function<hello>("cpp_hello", {
                    { php::TYPE_STRING },
                })
            - function<plus_5>("cpp_plus_5", {
                    { php::TYPE_INTEGER }, // 返回一个整数
                    { "ref", php::TYPE_INTEGER | php::BYREF}, // 接受一个整数参数
                })
            - function<call_method>("cpp_call_method", {
                    { php::TYPE_STRING }, // 返回整数
                    { "date", php::class_type("DateTime") }, // 指定名称的类作为参数类型
                })
            - function<container>("cpp_container", {
                    { php::TYPE_INTEGER },
                    { "key", php::TYPE_STRING },
                    { "val", php::FAKE_MIXED },
                })
            - function<invoke>("cpp_invoke", {
                    { php::FAKE_MIXED }, // 有返回值，但类型任意
                    { "callback", php::FAKE_CALLABLE }, // 虚拟的 Callable 类型作为参数
                })
            - function<walk>("cpp_walk", {
                    { php::TYPE_INTEGER },
                    { "array", php::TYPE_ARRAY }, // 数组参数
                })
            - function<conf_bytes>("cpp_conf_bytes", {
                    { php::TYPE_INTEGER },
                    { "unit", php::TYPE_STRING },
                })
            - function<property>("cpp_property", {
                    { php::FAKE_MIXED },
                    { "prop", php::class_type("DateInterval") },
                });
        
        return module;
    }
};
