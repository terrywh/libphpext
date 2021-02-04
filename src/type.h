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
        // 
        FAKE_NONE      = IS_UNDEF,
    };
    // 类型属性补充
    enum type_attr {
        NONE,
        ALLOW_NULL = 0x10,
        BYREF      = 0x20,
        VARIADIC   = 0x40,
    };
    class base_type {
    public:
        base_type(const type_code& code)
        : code_(code)
        , attr_(0) {}

        base_type(const base_type& bt) = default;
        //
        base_type operator |(const type_attr& attr) const {
            base_type bt(*this);
            bt.attr_ |= static_cast<int>(attr);
            return bt;
        }
        //
        inline std::uint32_t allow_null() const {
            return (attr_ & ALLOW_NULL) ? 1u : 0u;
        }
        //
        inline std::uint32_t pass_byref() const {
            return (attr_ & BYREF) ? 1u : 0u;
        }
        //
        inline std::uint32_t is_variadic() const {
            return (attr_ & VARIADIC) ? 1u : 0u;
        }
        // 
        operator zend_type() const {
            if(code_ > 0)
                return zend_type ZEND_TYPE_INIT_CODE(code_, allow_null(), _ZEND_ARG_INFO_FLAGS(pass_byref(), is_variadic()));
            else
                return zend_type ZEND_TYPE_INIT_NONE(0u);
        }
    private:
        uint32_t code_;
        uint32_t attr_;
    };
    class user_type {
    public:
        user_type(const char* name)
        : name_(name)
        , attr_(0) {}
        //
        user_type(const user_type& ut) = default;
        //
        user_type operator |(const type_attr& attr) const {
            user_type ut(*this);
            ut.attr_ |= static_cast<int>(attr);
            return ut;
        }
        // 用户 类 类型仅支持 ALLOW_NULL 标记
        inline std::uint32_t allow_null() const {
            return (attr_ & ALLOW_NULL) ? 1u : 0u;
        }
        // 用户 类 类型不支持 BYREF 标记
        inline std::uint32_t pass_byref() const {
            return 0u;
        }
        // 用户 类 类型不支持 VARIADIC 标记
        inline std::uint32_t is_variadic() const {
            return 0u;
        }
        // 
        operator zend_type() const {
            return zend_type ZEND_TYPE_INIT_CLASS_CONST(name_, allow_null(), 0u);
        }
    private:
        const char* name_;
        uint32_t    attr_;
    };
    using class_type = user_type;
    //
    base_type operator|(const type_code& code, const type_attr& attr);
    // 实现对 zend_type_list 的封装
    class type_list {
    public:
        type_list(const type_list& type_list) = default;
        operator zend_type() const {
            zend_type type { nullptr, 0u };
            zend_type_list* list = static_cast<zend_type_list*>(pemalloc(ZEND_TYPE_LIST_SIZE(types_.size()), true));
            list->num_types = types_.size();
            std::memcpy(&list->types, types_.data(), sizeof(zend_type) * types_.size());
            ZEND_TYPE_SET_LIST(type, list);
            return type;
        }
    private:
        std::vector<zend_type> types_;
        type_list() = default;
        // 参见 zend_API.c:2457 暂不支持此种类型注册内部函数
        // friend type_list operator |(const type_code& code1, const type_code& code2);
        // friend type_list operator |(const user_type& ut1,   const user_type& ut2);
        // friend type_list operator |(const type_list& tl1,  const user_type& ut2);
    };
    // 构建 union type
    // 参见 zend_API.c:2457 内部函数暂不支持此种类型注册内部函数
    // 构建 union type
    // type_list operator |(const type_code& code1, const type_code& code2);
    // 构建 union type
    // type_list operator |(const user_type& ut1, const user_type& ut2);
    // 加入 union type
    // type_list operator |(const type_list& tl1, const user_type& ut2);
    class type_desc {
    public:
        //
        type_desc(const type_code& code)
        : type_ (base_type{code}) {}
        // 
        type_desc(const base_type& base)
        : type_ (static_cast<zend_type>(base)) {}
        //
        type_desc(const user_type& user)
        : type_ (static_cast<zend_type>(user)) {}
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
