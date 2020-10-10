#include "../src/phpext.h"
#include "../src/vendor.h"

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
    return params[0].as<php::object>().call("format", {"Y-m-d H:i:s"});
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
    auto& array = params[0].as<php::array>();
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
    return php::environ::ini(params[0]).bytes();
}
// 访问属性
php::value property(php::parameters& params) {
    // 假设 d 原始值为 1
    auto d = params[0].as<php::object>().prop("d");
    // 函数设置：不会出发上述 d 值刷新
    params[0].as<php::object>().prop("d", 3);
    // 原地设置 -> 1 + 1 = 2
    d += 1;
    std::cout << d << " days\n";
    return d;
}

extern "C" {
    // PHP 扩展模块入口
    ZEND_DLEXPORT zend_module_entry* get_module() {
        // 构建模块：在离开函数后保留 module 定义
        static php::module_entry module("example1", "1.0.0");
        // 定义模块
        module
            // 模块依赖项
            .require("pcre", "8.0.0")
            .require("date")
            // 模块启动回调
            .on(php::module_startup([] (php::module_entry& module) -> bool {
                std::cout << "example1 --> module started" << std::endl;
                return true;
            }))
            .on(php::module_shutdown([] (php::module_entry& module) {
                std::cout << "example1 --> module shutdown" << std::endl;
            }))
            .on(php::request_startup([] (php::module_entry& module) -> bool {
                std::cout << "example1 --> request started" << std::endl;
                return true;
            }))
            .on(php::request_shutdown([] (php::module_entry& module) {
                std::cout << "example1 --> request shutdown" << std::endl;
            }))
            // 说明信息
            .describe("INFO_LIBPHPEXT_VERSION", LIBPHPEXT_VERSION)
            .describe("INFO_2", "Hello")
            // 配置 ini 项
            .setup("example.hello", "value") // 文本
            .setup("example.size", "2M") // 带单位
            // 定义常量
            .define("CPP_CONSTANT_1", "123456") // 字符串
            .define("CPP_CONSTANT_2", 123456) // 数值
            // 声明函数
            .declare<hello>("cpp_hello", {php::TYPE_STRING}) // 返回字符串
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
            }, {php::TYPE_INTEGER})
            .declare<conf_bytes>("cpp_conf_bytes", {
                {"name", php::TYPE_STRING}
            }, {php::TYPE_INTEGER})
            .declare<property>("cpp_property", {
                {"interval", "DateInterval"}
            }, {php::TYPE_MIXED});

        return module;
    }
};
