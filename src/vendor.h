#ifndef LIBPHPEXT_FORWARD_H
#define LIBPHPEXT_FORWARD_H

/** 系统头文件 **/
#include <cstdint>
#include <cstring>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

/** PHP 头文件 **/
#include <zend_API.h>
#include <zend_string.h>
#include <zend_exceptions.h>
#include <zend_smart_str.h>
#include <zend_inheritance.h>
#include <zend_interfaces.h>
#include <main/php.h>
#include <ext/standard/info.h>
extern "C" {
#include <ext/json/php_json.h>
}

#endif // LIBPHPEXT_FORWARD_H
