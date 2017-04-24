#pragma once

#include <exception>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <initializer_list>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

// using std::isnan;
extern "C" {
#include <zend.h>
#include <zend_API.h>
#include <zend_ini.h>
#include <zend_constants.h>
#include <zend_exceptions.h>
#include <zend_compile.h>
#include <zend_inheritance.h>
#include <zend_smart_str.h>
#include <zend_ini_scanner.h>

#include <php.h>
#include <standard/php_string.h>
#include <standard/url.h>
#include <standard/md5.h>
#include <standard/base64.h>
#include <standard/sha1.h>
#include <standard/crc32.h>
#include <json/php_json.h>
}
