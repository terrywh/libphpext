
### libphpext
libphpext is a library based on C++17 for writing PHP (v8.0) extensions; it encapsulates PHP's built-in types and objects that can be easily used to build PHP extensions; no third-party dependences.

### example
Below is a simple demo for baisc `libphpext` usage：

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
You will need a `php` installation to compile/link the **shared** library (see `xmake.lua` for how). After that put it to `php` extension dir, and modify `php.ini` to load it.

### Document
Please refer to
    * [`example/example1.cpp`](/terrywh/libphpext/blob/master/example/example1.cpp)
    * [`example/example2.cpp`](/terrywh/libphpext/blob/master/example/example2.cpp)
and comment in the libray header files.

### Copyright
This software uses the MIT license agreement.
