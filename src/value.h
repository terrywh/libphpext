#ifndef LIBPHPEXT_VALUE_H
#define LIBPHPEXT_VALUE_H

#include "forward.h"

namespace php {

    class object; // 对象
    class array;  // 数组

    // PHP 变量，对应 zval 结构
    template <bool ReleaseAfterUse = true>
    class value_t {
        template <bool RAU>
        friend class value_t;
    protected:
        mutable zval value_;
    public:
        // 变量类型
        enum types {
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
        value_t(const value_t<true>& v) {
            if constexpr (ReleaseAfterUse)
                ZVAL_COPY(&value_, Z_ISREF(v.value_) ? Z_REFVAL(v.value_) : &v.value_);
            else
                ZVAL_COPY_VALUE(&value_, &v.value_);
        }
        value_t(const value_t<false>& v) {
            if constexpr (ReleaseAfterUse)
                ZVAL_COPY(&value_, Z_ISREF(v.value_) ? Z_REFVAL(v.value_) : &v.value_);
            else
                ZVAL_COPY_VALUE(&value_, &v.value_);
        }
        value_t(value_t<true>&& v) {
            // 不要在 PHP 引用类型中使用移动构造
            assert(zval_get_type(&value_) != TYPE_REFERENCE); 
            ZVAL_COPY_VALUE(&value_, &v.value_);
            ZVAL_UNDEF(&v.value_);
        }
        value_t(value_t<false>&& v) {
            // 不要在 PHP 引用类型中使用移动构造
            assert(zval_get_type(&value_) != TYPE_REFERENCE); 
            ZVAL_COPY_VALUE(&value_, &v.value_);
            ZVAL_UNDEF(&v.value_);
        }
        // 未定义数据
        value_t() {
            ZVAL_UNDEF(&value_);
        }
        // NULL
        value_t(std::nullptr_t) {
            ZVAL_NULL(&value_);
        }
        value_t(bool b) {
            if(b) ZVAL_TRUE(&value_);
            else ZVAL_FALSE(&value_);
        }
        value_t(int i) {
            ZVAL_LONG(&value_, i);
        }
        value_t(std::int64_t i) {
            ZVAL_LONG(&value_, i);
        }
        operator std::int64_t() {
            return Z_LVAL(value_);
        }
        // 字符串
        value_t(std::string_view str) {
            ZVAL_NEW_STR(&value_, zend_string_init(str.data(), str.size(), 0));
        }
        // 字符串
        value_t(std::string str) {
            ZVAL_NEW_STR(&value_, zend_string_init(str.data(), str.size(), 0));
        }
        // 字符串
        value_t(zend_string* str, bool rau = ReleaseAfterUse) {
            if(rau) ZVAL_STR_COPY(&value_, str);
            else ZVAL_STR(&value_, str);
        }
        // 数组
        value_t(zend_array* arr, bool rau = ReleaseAfterUse) {
            ZVAL_ARR(&value_, arr);
            if(rau) addref();
        }
        // 对象
        value_t(zend_object* obj, bool rau = ReleaseAfterUse) {
            ZVAL_OBJ(&value_, obj);
            if(rau) addref();
        }
        // 类型判定
        inline bool is(types code) const {
            return zval_get_type(&value_) == code;
        }
        // 引用计数
        int addref() {
            assert(Z_REFCOUNTED(value_));
            return Z_ADDREF(value_);
        }
        // 引用计数
        int delref() {
            assert(Z_REFCOUNTED(value_));
            return Z_DELREF(value_);
        }
        ~value_t() {
            if constexpr (ReleaseAfterUse) zval_ptr_dtor(&value_);
        }
        
        // 类型数据
        inline operator zval*() const {
            return &value_;
        }
        // 类型数据
        operator bool() const {
            assert(zval_get_type(&value_) == TYPE_TRUE || zval_get_type(&value_) == TYPE_FALSE);
            return zval_get_type(&value_) == TYPE_TRUE;
        }
        // 类型数据
        operator std::int64_t() const {
            assert(zval_get_type(&value_) == TYPE_INTEGER);
            return Z_LVAL(value_);
        }
        // 类型数据
        operator double() const {
            assert(zval_get_type(&value_) == TYPE_FLOAT);
            return Z_DVAL(value_);
        }
        // 类型数据：字符串
        operator zend_string*() const {
            assert(zval_get_type(&value_) == TYPE_STRING);
            return Z_STR(value_);
        }
        // 类型数据：字符串
        operator std::string_view() const { // 返回 string_view 可直接访问内存
            assert(zval_get_type(&value_) == TYPE_STRING);
            return {Z_STRVAL(value_), Z_STRLEN(value_)};
        }
        // 类型数据：字符串
        operator std::string() const {
            assert(zval_get_type(&value_) == TYPE_STRING);
            return {Z_STRVAL(value_), Z_STRLEN(value_)};
        }
        // 类型数据：数组
        operator zend_array*() const {
            assert(zval_get_type(&value_) == TYPE_ARRAY);
            return Z_ARR(value_);
        }
        // 数据类型：对象
        operator zend_object*() const {
            assert(zval_get_type(&value_) == TYPE_OBJECT);
            return Z_OBJ(value_);
        }
        // 运算符：赋值
        value_t& operator =(const value_t& v) {
            zval_ptr_dtor(&value_);
            ZVAL_COPY(&value_, &v.value_);
            return *this;
        }
        // 运算符：赋值
        value_t& operator =(value_t&& v) {
            zval_ptr_dtor(&value_);
            ZVAL_COPY_VALUE(&value_, &v.value_);
            ZVAL_UNDEF(&v.value_);
            return *this;
        }
        // 运算符：赋值
        value_t& operator +=(int x) {
            assert(zval_get_type(&value_) == TYPE_INTEGER);
            Z_LVAL(value_) += x;
            return *this;
        }
        // 运算符：自增
        value_t& operator ++() {
            assert(zval_get_type(&value_) == TYPE_INTEGER);
            ++Z_LVAL(value_);
            return *this;
        }
        // 运算符：赋值
        value_t& operator -=(int x) {
            assert(zval_get_type(&value_) == TYPE_INTEGER);
            Z_LVAL(value_) -= x;
            return *this;
        }
        // 运算符：自减
        value_t& operator --() {
            assert(zval_get_type(&value_) == TYPE_INTEGER);
            --Z_LVAL(value_);
            return *this;
        }
        // 运算符：相等
        bool operator ==(const value_t& v) const {
            value_t r;
            return is_equal_function(&r.value_, &value_, &v.value_);
        }
        // 运算符：相等
        bool operator ==(int v) const {
            assert(zval_get_type(&value_) == TYPE_INTEGER);
            return Z_LVAL(value_) == v;
        }
        // 运算符：相等
        bool operator ==(std::int64_t v) const {
            assert(zval_get_type(&value_) == TYPE_INTEGER);
            return Z_LVAL(value_) == v;
        }
        // 运算符：相等
        bool operator ==(std::string_view v) const {
            return zval_get_type(&value_) == TYPE_STRING && 
                zend_binary_strcmp(Z_STRVAL(value_), Z_STRLEN(value_), v.data(), v.size()) == 0;
        }
        // 运算符：比较
        bool operator <(const value_t& v) const {
            value_t r;
            return compare_function(&r.value_, &value_, &v.value_) < 0;
        }
        // 运算符：比较
        bool operator <(int v) const {
            assert(zval_get_type(&value_) == TYPE_INTEGER);
            return Z_LVAL(value_) > v;
        }
        // 运算符：比较
        bool operator <(std::int64_t v) const {
            assert(zval_get_type(&value_) == TYPE_INTEGER);
            return Z_LVAL(value_) > v;
        }
        // 运算符：比较
        bool operator <(std::string_view v) const {
            assert(zval_get_type(&value_) == TYPE_STRING);
            return zend_binary_strcmp(Z_STRVAL(value_), Z_STRLEN(value_), v.data(), v.size()) < 0;
        }
        // 访问内部类型对象 (方法)
        template <class T>
        T* as() const noexcept {
            assert(zval_get_type(&value_) == T::TYPE);
            return reinterpret_cast<T*>(Z_REFCOUNT_P(&value_));
        }
    };
    using value = value_t<true>; // 
}

#endif // LIBPHPEXT_VALUE_H
