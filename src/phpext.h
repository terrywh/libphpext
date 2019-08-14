#pragma once

#define PHPEXT_LIB_VERSION "2.2.1"
#include "vendor.h"

#include "type.h"
#include "class.h"
#include "error_info.h"
#include "error.h" // -> error_info
#include "buffer.h"
#include "stream_buffer.h"
#include "value.h" // -> type
#include "exception.h" // -> error exception
#include "string.h" // -> value buffer
#include "value_fn.h" // -> value exception string
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
