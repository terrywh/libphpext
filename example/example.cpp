#include "../src/phpext.h"
#include <iostream>
#include <map>
#include <unordered_map>

// 所有导出到 PHP 的函数必须符合下面形式：
php::value hello(php::parameters& params) {
    php::process_title("hello world");
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
    return php::env::ini(params[0]).bytes();
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

// attribute
class attribute {
public:
    php::value __construct(php::parameters& params) {
        std::cout << "http " << params[0] << " " << params[1] << std::endl;
        return nullptr;
    }
};

class example {
public:
    php::value count(php::parameters& params) {
        // 获取 PHP 对象（方法1）
        auto* obj = php::class_entry<example>::native(this);
        return obj->prop("prop1"); // 读取属性
    }
    // 同步属性（方法2）
    php::member prop3;

    php::value hello(php::parameters& params) {
        // 字符串拼接
        php::string_builder sb;
        sb << "hello " << params[0];
        // 同步属性设置
        prop3 = "modified";
        return sb.str();
    }
    // 静态方法
    static php::value number(php::parameters& params) {
        return 123;
    }
};

class example2: public php::class_basic<example2> {
public:
    php::value __construct(php::parameters& params) {
        // 获取 PHP 对象属性（方法3）
        std::cout << prop("prop1") << std::endl;
        return nullptr;
    }
};

extern "C" {
    // PHP 扩展模块入口
    ZEND_DLEXPORT zend_module_entry* get_module() {
        // 构建模块：在离开函数后保留 module 定义
        static php::module_entry module("ext_example", "1.0.0");
        // 定义模块
        module
            // 模块依赖项
            .require("pcre", "8.0.0")
            .require("date")
            // 模块启动回调
            .on(php::module_startup([] (php::module_entry& module) -> bool {
                std::cout << "--> module started" << std::endl;
                return true;
            }))
            .on(php::module_shutdown([] (php::module_entry& module) {
                std::cout << "--> module shutdown" << std::endl;
            }))
            .on(php::request_startup([] (php::module_entry& module) -> bool {
                std::cout << "--> request started" << std::endl;
                return true;
            }))
            .on(php::request_shutdown([] (php::module_entry& module) {
                std::cout << "--> request shutdown" << std::endl;
                return true;
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
            }, {php::TYPE_INTEGER})
            .declare<conf_bytes>("cpp_conf_bytes", {
                {"name", php::TYPE_STRING}
            }, {php::TYPE_INTEGER})
            .declare<property>("cpp_property", {
                {"interval", "DateInterval"}
            }, {php::TYPE_MIXED});
        // 声明一个类
        module.declare<attribute>("cpp_attribute")
            .attribute() // 这是一个 attribute
            .declare<&attribute::__construct>("__construct", {
                {"method", php::TYPE_STRING},
                {"path", php::TYPE_STRING},
            });
        // 声明一个类
        module.declare<example>("cpp_example") // 标记 attribute 定义
            .attribute("cpp_attribute", {"POST", "/hello"}) // 标记 attribute 定义
            .define("CONST_1", "abc") // 类常量
            .implements(&zend_ce_countable) // 实现接口
            .declare<&example::count>("count", {}, {php::TYPE_INTEGER}) // 接口方法
            .declare<&example::hello>("hello", { // 普通方法
                {"name", php::TYPE_STRING}
            }, {php::TYPE_STRING})
            .declare<example::number>("number", {}, {php::TYPE_INTEGER}) // 静态方法
            .declare("prop1", 123) // 属性
            .declare("prop2", "static_value", true) // 静态属性
            // 同步属性：参数 #3 成员指针实际并未使用，仅用于类型检查，必须提供 #4 属性偏移
            .declare("prop3","should_be_changed", &example::prop3, offsetof(example, prop3));
        
        module.declare<example2>("cpp_example2")
            .declare("prop1", 123)
            .declare<&example2::__construct>("__construct");
        return module;
    }
};
