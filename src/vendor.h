#ifndef LIBPHPEXT_VENDOR_H
#define LIBPHPEXT_VENDOR_H

/** 系统头文件 **/
#include <cstdint>
#include <cstring>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

/** PHP 头文件 **/
#include <zend_API.h>
#include <zend_string.h>
#include <zend_exceptions.h>
#include <zend_smart_str.h>
#include <zend_inheritance.h>
#include <zend_interfaces.h>
#include <zend_attributes.h>
#include <main/php.h>
#include <main/php_variables.h>
extern "C" {
#include <ext/json/php_json.h>
#include <ext/standard/url.h>
}

#endif // LIBPHPEXT_VENDOR_H
