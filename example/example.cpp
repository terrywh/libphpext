#include "../src/phpext.h"
#include <iostream>
#include <map>
#include <unordered_map>

// 所有导出到 PHP 的函数必须符合下面形式：
php::value hello(php::parameters& params) {
    // 参数下标必须存在, 否则会抛出异常
    php::value v0 = params[0];
    return "hello " + static_cast<std::string>(v0); // 常用内部类型双向转换
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
    return params[0].as<php::object>()->call("format", {"Y-m-d H:i:s"});
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
    return params[0]({"world"});
}
// 遍历数组
php::value walk(php::parameters& params) {
    auto* array = params[0].as<php::array>();
    int count = array->size();
    for(auto i = array->begin(); i!= array->end(); ++i) {
        std::cout << i->first << " => " << i->second << std::endl;
        ++count;
    }
    // C++11
    // for(auto x : *params[0].as<php::array>()) {
    //     std::cout << x.first << " => " << x.second << std::endl;
    //     ++count;
    // }
    return array->size();
}

extern "C" {
    // PHP 扩展模块入口
    ZEND_DLEXPORT zend_module_entry* get_module() {
        // 构建模块：在离开函数后保留 module 定义
        static php::module_entry module("ext_example", "1.0.0");
        // 定义模块
        module
            // 模块依赖项
            .depend({"pcre", "ge", "8.0.0", MODULE_DEP_REQUIRED})
            // 模块启动回调
            .on(php::module_startup([] (php::module_entry& module) -> bool {
                std::cout << "module started" << std::endl;
                return true;
            }))
            .on(php::module_shutdown([] (php::module_entry& module) {
                // 工作在 CLI 模式下时不被执行
                std::cout << "module shutdown" << std::endl;
            }))
            .on(php::request_startup([] (php::module_entry& module) -> bool {
                std::cout << "request started" << std::endl;
                return true;
            }))
            .on(php::request_shutdown([] (php::module_entry& module) {
                std::cout << "request shutdown" << std::endl;
                return true;
            }))
            // 说明信息
            .descibe({"INFO_LIBPHPEXT_VERSION", LIBPHPEXT_VERSION_STRING})
            .descibe({"INFO_2", "Hello"})
            // 定义常量
            .define({"CPP_CONSTANT_1", "123456"}) // 字符串
            .define({"CPP_CONSTANT_2", 123456}) // 数值
            // 声明函数
            .declare<hello>("cpp_hello", {
                {"s1", php::TYPE_STRING} // 接受一个字符串参数
            }, {php::TYPE_STRING}) // 返回字符串
            .declare<plus_5>("cpp_plus_5", { 
                {"i1", php::TYPE_INTEGER, true} // 接受一个整数参数
            }, {php::TYPE_INTEGER}) // 返回一个整数
            .declare<call_method>("cpp_call_method", {
                {"o1", "DateTime"}
            }, {php::TYPE_STRING})
            .declare<container>("cpp_container", {
                {"k", php::TYPE_STRING},
                {"v", php::TYPE_MIXED}
            }, {php::TYPE_INTEGER})
            .declare<invoke>("cpp_invoke", {
                {"cb", php::FAKE_CALLABLE}
            }, {php::TYPE_MIXED})
            .declare<walk>("cpp_walk", {
                {"a", php::TYPE_ARRAY}
            }, {php::TYPE_INTEGER});

        return module;
    }
};
