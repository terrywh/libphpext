// #include "../src/phpext.h"
#include "../src/vendor.h"
#include <iostream>
// #include <map>
// #include <unordered_map>

// // 所有导出到 PHP 的函数必须符合下面形式：
// php::value hello(php::parameters& params) {
//     php::process_title("hello world");
//     // 参数下标必须存在, 否则会抛出异常
//     php::value v0 = params[0];
//     return "hello " + static_cast<std::string>(v0); // 常用内部类型双向转换
// }
// // 引用型参数可改变其值
// php::value plus_5(php::parameters& params) {
//     php::value  x = params[0]; // 复制
//     php::value& y = params[0]; // 引用
    
//     x += 5; // 本地复制
//     y += 5; // 修改参数
//     // 也可直接对原始参数赋值
//     // params[0] += 5;
//     return x;
// }
// // 调用对象函数
// php::value call_method(php::parameters& params) {
//     return params[0].as<php::object>().call("format", {"Y-m-d H:i:s"});
// }
// // 放入 C++ 容器
// php::value container(php::parameters& params) {
//     std::map<php::value, php::value> c1;
//     std::unordered_map<php::value, php::value> c2;

//     c1[params[0]] = params[1]; // Sortable
//     c2[params[0]] = params[1]; // Hashable

//     return std::int64_t(c1.size() + c2.size());
// }
// // 调用回调
// php::value invoke(php::parameters& params) {
//     return params[0]({"world"});
// }
// // 遍历数组
// php::value walk(php::parameters& params) {
//     auto& array = params[0].as<php::array>();
//     int count = array.size();
//     // >= C++11
//     // for(auto x : array) {
//     //     std::cout << x.first << " => " << x.second << std::endl;
//     //     ++count;
//     // }
//     for(auto i = array.begin(); i!= array.end(); ++i) {
//         std::cout << i->first << " => " << i->second << std::endl;
//         ++count;
//     }
//     // for(auto i = array.rbegin(); i!= array.rend(); ++i) {
//     //     std::cout << i->first << " => " << i->second << std::endl;
//     //     ++count;
//     // }
//     return count;
// }
// // 读取配置
// php::value conf_bytes(php::parameters& params) {
//     return php::env::ini(params[0]).bytes();
// }
// // 访问属性
// php::value property(php::parameters& params) {
//     // 假设 d 原始值为 1
//     auto d = params[0].as<php::object>().prop("d");
//     // 函数设置：不会出发上述 d 值刷新
//     params[0].as<php::object>().prop("d", 3);
//     // 原地设置 -> 1 + 1 = 2
//     d += 1;
//     std::cout << d << " days\n";
//     return d;
// }

// // attribute
// class attribute {
// public:
//     php::value __construct(php::parameters& params) {
//         std::cout << "http " << params[0] << " " << params[1] << std::endl;
//         return nullptr;
//     }
// };

// class example {
// public:
//     php::value count(php::parameters& params) {
//         // 获取 PHP 对象（方法1）
//         auto* obj = php::class_entry<example>::native(this);
//         return obj->prop("prop1"); // 读取属性
//     }
//     // 同步属性（方法2）
//     php::member prop3;

//     php::value hello(php::parameters& params) {
//         // 字符串拼接
//         php::string_builder sb;
//         sb << "hello " << params[0];
//         // 同步属性设置
//         prop3 = "modified";
//         return sb.str();
//     }
//     // 静态方法
//     static php::value number(php::parameters& params) {
//         return 123;
//     }
// };

// class example2: public php::class_basic<example2> {
// public:
//     php::value __construct(php::parameters& params) {
//         // 获取 PHP 对象属性（方法3）
//         std::cout << prop("prop1") << std::endl;
//         return nullptr;
//     }
// };

extern "C" {
    // PHP 扩展模块入口
    ZEND_DLEXPORT zend_module_entry* get_module() {
        std::ios_base::sync_with_stdio(false);
        php_printf("0: %d\n",errno);
        std::cout << "example3 --> before" << std::endl;
        php_printf("1: %d\n",errno);
        std::cout << "3001" << std::endl;
        std::cout << 3002 << std::endl;
        php_printf("2: %d\n",errno);
        std::cout << "example3 --> after" << std::endl;
        php_printf("3: %d\n",errno);
        
        static zend_module_entry module {
            STANDARD_MODULE_HEADER_EX,
            nullptr, // 配置 ini entry 此项数据由 zend engine 填充
            nullptr, // 依赖模块，暂不设置 --> (1)
            "example3", // 模块名称
            nullptr, // 函数表，暂不设置 --> (1)
            nullptr, // on_module_startup,
            nullptr, // on_module_shutdown,
            nullptr, // on_request_startup,
            nullptr, // on_request_shutdown,
            nullptr, // on_module_info,
            "1.0.0", // 版本号
            STANDARD_MODULE_PROPERTIES,
        };
        return &module;
    }
};
