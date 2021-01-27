#ifndef LIBPHPEXT_VENDOR_H
#define LIBPHPEXT_VENDOR_H

/** 系统头文件 **/
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

/** PHP 头文件 **/
extern "C" {
    #include <main/php.h>
    #include <main/php_variables.h>
    #include <zend_string.h>
    #include <zend_exceptions.h>
    #include <zend_smart_str.h>
    #include <zend_inheritance.h>
    #include <zend_interfaces.h>
    #include <zend_attributes.h>
    #include <ext/standard/php_array.h>
    #include <ext/standard/php_http.h>
    #include <ext/standard/info.h>
    #include <ext/standard/url.h>
    #include <ext/json/php_json.h>
}

#endif // LIBPHPEXT_VENDOR_H
