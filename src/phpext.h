#pragma once

#ifdef NDEBUG
#define PHPEXT_NDEBUG
#endif

#include <exception>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <initializer_list>
#include <list>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <cmath>
using std::isfinite;
// using std::isnan;
extern "C" {
#include <main/php.h>
#include <main/php_variables.h>
#include <ext/standard/info.h>
#include <ext/standard/php_string.h>
#include <ext/standard/php_array.h>
#include <ext/standard/php_http.h>
#include <ext/standard/url.h>
#include <ext/standard/md5.h>
#include <ext/standard/base64.h>
#include <ext/standard/sha1.h>
#include <ext/standard/crc32.h>
#include <ext/json/php_json.h>

#include <Zend/zend.h>
#include <Zend/zend_API.h>
#include <Zend/zend_ini.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_constants.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_compile.h>
#include <Zend/zend_inheritance.h>
#include <Zend/zend_smart_str.h>
#include <Zend/zend_ini_scanner.h>
#include <Zend/zend_generators.h>
#include <Zend/zend_closures.h>
#include <Zend/zend_operators.h>
#include <Zend/zend_builtin_functions.h>
}

#ifdef PHPEXT_NDEBUG
#define NDEBUG
#else
#undef NDEBUG
#endif
#include <cassert>

#include "type.h"
#include "class.h"
#include "error_info.h"
#include "error.h" // -> error_info
#include "buffer.h"
#include "value.h" // -> type
#include "string.h" // -> value buffer
#include "exception.h" // -> error exception
#include "parameters.h" // -> value exception
#include "property.h" // -> value string
#include "array_member.h" // -> value string
#include "array_iterator.h" // -> value string array_member
#include "object.h" // -> value string property
#include "class_base.h" // -> object
#include "array.h" // -> value string array_member array_iterator
#include "callable.h" // -> value array
#include "closure.h" // -> class_base object callable
#include "class_wrapper.h"
#include "arguments.h"
#include "specifier.h"
#include "delegate.h"
#include "property_entry.h"
#include "constant_entry.h"
#include "class_entry.h"
#include "ini_entry.h"
#include "extension_entry.h"
#include "util.h"
#include "ini.h"
#include "global.h"

