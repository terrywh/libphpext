
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
#include <ostream>

using std::isfinite;

#ifdef NDEBUG
#define PHPEXT_NDEBUG
#undef NDEBUG
#endif

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
#include <ext/date/php_date.h>

#include <Zend/zend_types.h>
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

#include "macro.h"

#ifdef PHPEXT_NDEBUG
#define NDEBUG
#endif
#include <cassert>