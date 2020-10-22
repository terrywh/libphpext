#include "../phpext.h"
#include <iostream>

class cpp_attribute {
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
    php::property_refer prop3;

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
// 继承基类，可使用部分辅助方法
class example2: public php::class_basic<example2> {
public:
    php::value __construct(php::parameters& params) {
        // 获取 PHP 对象属性（方法3）
        std::cout << prop("prop1") << std::endl;
        return nullptr;
    }
};

using php::attribute;
using php::method;
using php::constant;
using php::static_method;
using php::implement;
using php::property;
using php::static_property;

extern "C" {
    // PHP 扩展模块入口
    ZEND_DLEXPORT zend_module_entry* get_module() {
        // 构建模块：在离开函数后保留 module 定义
        static php::module_entry module("example2", "1.0.0");
        // 声明一个类
        module.declare<cpp_attribute>("cpp_attribute")
            - php::attribute() // 当前类型可用于 attribute 元数据描述
            - method<&cpp_attribute::__construct>("__construct", {
                {"method", php::TYPE_STRING},
                {"path", php::TYPE_STRING},
            });
        // 声明一个类
        module.declare<example>("cpp_example") // 标记 attribute 定义
            - attribute("cpp_attribute", {"POST", "/hello"}) // 标记 attribute 定义
            - constant("CONST_1", "abc") // 类常量
            - implement(&zend_ce_countable) // 实现接口
            - method<&example::count>("count", {}, {php::TYPE_INTEGER}) // 接口方法
            - method<&example::hello>("hello", { // 普通方法
                    {"name", php::TYPE_STRING}
                }, {php::TYPE_STRING})
            - static_method<example::number>("number", {php::TYPE_INTEGER}) // 静态方法
            - property("prop1", 123) // 属性
            - static_property("prop2", "static_value") // 静态属性
            - property("prop3", "should_be_changed", &example::prop3); // 同步属性
        
        return module;
    }
};
