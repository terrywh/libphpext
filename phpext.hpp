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

#include "type.hpp"
#include "class.hpp"
#include "error_info.hpp"
#include "buffer.hpp"
#include "value.hpp" // -> type
#include "string.hpp" // -> value buffer
#include "exception.hpp" // -> error exception
#include "parameters.hpp" // -> value exception
#include "value_impl.hpp" // -> value exception string
#include "class_impl.hpp" // -> class string exception
#include "property.hpp" // -> value string
#include "array_member.hpp" // -> value string
#include "array_iterator.hpp" // -> value string array_member
#include "object.hpp" // -> value string property
#include "exception_impl.hpp" // -> throwable object string
#include "class_base.hpp" // -> object
#include "array.hpp" // -> value string array_member array_iterator
#include "callable.hpp" // -> value array

#include "class_wrapper.hpp"
#include "arguments.hpp"
#include "specifier.hpp"
#include "delegate.hpp"
#include "property_entry.hpp"
#include "constant_entry.hpp"
#include "class_base.hpp"
#include "class_entry.hpp"
#include "ini_entry.hpp"
#include "closure.hpp"
#include "extension_entry.hpp"
#include "util.hpp"
#include "ini.hpp"

