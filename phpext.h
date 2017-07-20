#pragma once

#include "vendor.h"

#include "bases/class_base.h"
#include "bases/class_builtin.h"

#include "types/exception.h"
#include "types/value.h"
#include "types/callable.h"
#include "types/buffer.h"
#include "types/string.h"
#include "types/array.h"
#include "types/object.h"
#include "types/generator.h"
#include "types/callable.h"
#include "types/parameters.h"

#include "parts/constant_entry.h"
#include "parts/ini_entry.h"
#include "parts/arguments.h"
#include "parts/property_entry.h"
#include "parts/class_wrapper.h"
#include "parts/class_entry.h"
#include "parts/method_entry.h"
#include "parts/function_entry.h"
#include "parts/extension_entry.h"

#include "funcs/util.h"
#include "funcs/string_builder.h"
#include "funcs/string_parser.h"
#include "funcs/parse_url.h"
#include "funcs/parse_ini.h"
#include "funcs/hash.h"
#include "funcs/encoding.h"
#include "funcs/reference.h"
// 扩展 so 中需要定义下面函数
extern void extension_init(php::extension_entry& extension);
// 链接 so 时须指定 -u get_module 参数以导出该符号
