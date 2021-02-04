#ifndef LIBPHPEXT_ARGUMENT_ENTRY_H
#define LIBPHPEXT_ARGUMENT_ENTRY_H

#include "vendor.h"
#include "type.h"

#define PHP_DEFAULT_VALUE(expr) #expr

namespace php {
    class argument_entry {
    public:
        argument_entry(const char* name, type_desc desc, const char* default_value = nullptr)
        : info_ {name, desc, default_value} {}

        argument_entry(type_desc desc, const char* name, const char* default_value = nullptr)
        : info_ {name, desc, default_value} {}
        // 用于返回值描述
        argument_entry(type_desc desc)
        : info_ {nullptr, desc, nullptr} {}

        bool allow_null() {
            return ZEND_TYPE_ALLOW_NULL(info_.type);
        }
        operator zend_internal_arg_info() const {
            return info_;
        }
    private:
        zend_internal_arg_info info_;
    };
    // 
    using return_entry = argument_entry;
}

#endif // LIBPHPEXT_ARGUMENT_ENTRY_H