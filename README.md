### libphpext
基于 C++17 封装的用于编写 PHP (v8.0) 扩展的辅助库；不依赖其他任何第三方库，将 PHP 内置的类型、对象、方法提取、整理封装成能够简单使用的 C++ 类对象，并提供相关扩展组装机制，以此简化 PHP 扩展开发工作。

### 示例
下面示例简单展示了 libphpext 的最基本用法：

``` C++
#include <phpext.h> // 应用库统一的头文件
php::value hello(php::parameters& params) { // 定义一个函数
    return "hello, " + static_cast<std::string>(params[0]);
}
class example { // 定义一个类
private:
    static int count_;
public:
    php::value __construct(php::parameters& params) { // PHP 构造函数
        ++ count_; // 对象构建数量
        return nullptr;
    }
};
int example::count_ = 0;
// 导出 PHP 扩展
extern "C" {
    ZEND_DLEXPORT zend_module_entry* get_module() {
        static php::module_entry entry("example", "1.0.0"); // 定义 名称及版本（须保持在内存中）
        entry
            - php::require("json") // 依赖其他扩展
            - php::function<hello>("hello", { // 函数
                php::TYPE_STRING       // 返回：字符串
                php::TYPE_STRING       // 参数：字符串
            });
        entry.declare<example>("example") // 类导出
            - php::property("property1", 123456) // 属性
            - php::method<&example::__construct>("__construct"); // 构造函数
        return module; // 完工
    }
}
```
你需要一份 `PHP` 安装才能正确编译链接动态库；然后将库文件放置到 `PHP` 扩展路径中并在 `php.ini` 中启用加载即可。

### 文档
请参考示例扩展源代码： 
    * [`example/example1.cpp`](/terrywh/libphpext/blob/master/example/example1.cpp)
    * [`example/example2.cpp`](/terrywh/libphpext/blob/master/example/example2.cpp)
及对应的库头文件、相关注释；

### 版权声明
本软件使用 MIT 许可协议。