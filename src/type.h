#ifndef LIBPHPEXT_TYPE_H
#define LIBPHPEXT_TYPE_H

#include "vendor.h"

namespace php {
    // 变量类型
    enum type_code: unsigned int {
        FAKE_MIXED     = IS_MIXED,
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
        FAKE_REFERENCE = IS_REFERENCE,
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
    // 类型属性补充
    enum type_attr {
        NONE,
        ALLOW_NULL = 0x10,
        BYREF      = 0x20,
        VARIADIC   = 0x40,
    };
    class type_desc;
    //
    class type_item {
        const std::uint32_t IS_CLASS_NAME = 0x01;
    public:
        explicit type_item(const char* name)
        : attr_(IS_CLASS_NAME),name_(name) {}

        type_item(const type_code& code)
        : attr_(NONE), code_(code) {}
        // 
        type_item(const type_item& ti) = default;
        // 
        operator zend_type() const;
        //
        type_item operator |(const type_attr& attr) const;
        //
        inline std::uint32_t allow_null() const {
            return attr_ & ALLOW_NULL;
        }
        //
        inline std::uint32_t pass_byref() const {
            return attr_ & BYREF;
        }
        //
        inline std::uint32_t is_variadic() const {
            return attr_ & VARIADIC;
        }
    private:
        uint32_t  attr_ = 0;
        union {
            uint32_t    code_;
            const char* name_;
        };
        friend type_item operator|(const type_code& code, const type_attr& attr);
    };
    using class_type = type_item;
    //
    type_item operator|(const type_code& code, const type_attr& attr);
    // 实现对 zend_type_list 的封装
    class type_list {
    public:
        type_list() = default;
        type_list(const type_list& type_list) = default;
        operator zend_type() const;
    private:
        std::vector<zend_type> types_;
        friend type_list operator |(const type_code& code1, const type_code& code2);
        friend type_list operator |(const type_desc& code1, const type_code& code2);
        friend type_list operator |(const type_list& list,  const type_code& code2);
    };
    // 构建 union type
    type_list operator |(const type_code& code1, const type_code& code2);
    // 构建 union type
    type_list operator |(const type_desc& code1, const type_code& code2);
    // 加入 union type
    type_list operator |(const type_list& list,  const type_code& code2);

    class type_desc {
    public:
        //
        type_desc(const type_code& code)
        : type_ ZEND_TYPE_INIT_CODE(code, 0u, _ZEND_ARG_INFO_FLAGS(0u, 0u)) { }
        // 
        type_desc(const type_item& item)
        : type_ (static_cast<zend_type>(item)) {}
        // 
        type_desc(const type_list& list)
        : type_ (static_cast<zend_type>(list)) {}

        operator zend_type() const {
            return type_;
        }
    private:
        zend_type type_;
    };
}
#endif  // LIBPHPEXT_TYPE_H
