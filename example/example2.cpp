#include "../src/phpext.h"
#include <iostream>

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
        static php::module_entry module("example1", "1.0.0");
        // 定义模块
        module
            // 模块依赖项
            .require("pcre", "8.0.0")
            .require("date");
            
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
