
### libphpext
libphpext is a library based on C++17 for writing PHP (v8.0) extensions; it encapsulates PHP's built-in types and objects that can be easily used to build PHP extensions; no third-party dependences.

### example
Below is a simple demo for baisc `libphpext` usage：

``` C++
#include <phpext.h>
php::value hello(php::parameters& params) { // define a c++ / php function
    return "hello, " + static_cast<std::string>(params[0]);
}
class example { // define a class
private:
    static int count_;
public:
    php::value __construct(php::parameters& params) { // constructor for php object
        ++ count_;
        return nullptr;
    }
};
int example::count_ = 0;
extern "C" { // export a php extension:
    ZEND_DLEXPORT zend_module_entry* get_module() {
        static php::module_entry module("example", "1.0.0"); // define a extension module
        module
            .require("json") // depending on other extension ?
            .declare<hello>("hello", {"who", php::TYPE_STRING}, {php::TYPE_STRING}); // function
        module.declare<example>("example") // class
            .declare("property1", 123456) // property
            .declare<&example::__construct>("__construct"); // constructor is also a method
        return module; // done
    }
}
```
You will need a `php` installation to compile/link the **shared** library. After that put it to `php` extension dir, and modify `php.ini` to load it.

### Document
Please refer to [`example/example.cpp`](/terrywh/libphpext/blob/master/example/example.cpp) / [`example/example.php`](/terrywh/libphpext/blob/master/example/example.php) and comment in the libray header files.

### Copyright
This software uses the MIT license agreement.
