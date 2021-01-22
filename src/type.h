#ifndef LIBPHPEXT_TYPE_H
#define LIBPHPEXT_TYPE_H

#include "vendor.h"

namespace php {
    // 变量类型
    enum type_code: unsigned int {
        TYPE_MIXED     = IS_MIXED,
        // 标准类型
        TYPE_UNDEFINED = IS_UNDEF,
        // 标准类型
        TYPE_NULL      = IS_NULL,
        // 标准类型
        TYPE_FALSE	   = IS_FALSE,
        // 标准类型
        TYPE_TRUE      = IS_TRUE,
        // 标准类型
        TYPE_LONG      = IS_LONG,
        // 标准类型
        TYPE_INTEGER   = IS_LONG,
        // 标准类型
        TYPE_FLOAT     = IS_DOUBLE,
        // 标准类型
        TYPE_DOUBLE    = IS_DOUBLE,
        // 标准类型
        TYPE_STRING    = IS_STRING,
        // 标准类型
        TYPE_ARRAY     = IS_ARRAY,
        // 标准类型
        TYPE_OBJECT    = IS_OBJECT,
        // 标准类型
        TYPE_RESOURCE  = IS_RESOURCE,
        // 标准类型
        TYPE_REFERENCE = IS_REFERENCE,
        // 常量表达式
        TYPE_CONSTANT_AST = IS_CONSTANT_AST,
        // 内部类型
        TYPE_INDIRECT  = IS_INDIRECT,
        // 内部类型
        TYPE_POINTER   = IS_PTR,
        // 内部类型
        TYPE_PTR       = IS_PTR,
        // 内部类型
        TYPE_ALIAS_PTR = IS_ALIAS_PTR,
        // 内部类型
        FAKE_ERROR     = _IS_ERROR,
        // 实际不存在此类型，仅用于参数说明
        FAKE_BOOLEAN   = _IS_BOOL, 
        // 实际不存在此类型，仅用于参数说明
        FAKE_CALLABLE  = IS_CALLABLE,
        // 实际不存在次类型，仅用于参数说明
        FAKE_ITERABLE  = IS_ITERABLE,
        // 实际不存在次类型，仅用于参数说明
        FAKE_VOID      = IS_VOID,
        // 实际不存在次类型，仅用于参数说明
        FAKE_NUMBER    = _IS_NUMBER,
    };
    class type_list;
    // 简单对 zend_type 封装
    class type_desc {
    public:
        type_desc(type_code    type_hint, bool nullable = false, bool byref = false, bool variadic = false);
        type_desc(zend_class_entry*   ce, bool nullable = false);
        type_desc(const char* class_name, bool nullable = false);
        operator zend_type() const {
            return type_;
        }
        type_list operator |(const type_desc& desc) const;
    private:
        zend_type type_;
    };
    // 实现对 zend_type_list 的封装
    class type_list {
    public:
        type_list() {}
        type_list(type_list&& type_list) = default;
        type_list operator |(const type_desc& desc) const;
        operator zend_type() const;
    private:
        std::vector<type_desc> types_;
    };
}
#endif  // LIBPHPEXT_TYPE_H
