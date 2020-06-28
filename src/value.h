#ifndef LIBPHPEXT_VALUE_H
#define LIBPHPEXT_VALUE_H

#include "vendor.h"
#include "type_code.h"
#include "string.h"
#include "object.h"
#include "array.h"
#include "callable.h"

namespace php {

    class object; // 对象

    // PHP 变量，对应 zval 结构
    template <bool ReleaseAfterUse = true>
    class value_t {
        template <bool RAU>
        friend class value_t;
    protected:
        mutable zval value_;
    public:
        // 构造：复制
        value_t(const value_t<true>& v) {
            if constexpr (ReleaseAfterUse)
                ZVAL_COPY(&value_, Z_ISREF(v.value_) ? Z_REFVAL(v.value_) : &v.value_);
            else
                ZVAL_COPY_VALUE(&value_, &v.value_);
        }
        // 构造：复制
        value_t(const value_t<false>& v) {
            if constexpr (ReleaseAfterUse)
                ZVAL_COPY(&value_, Z_ISREF(v.value_) ? Z_REFVAL(v.value_) : &v.value_);
            else
                ZVAL_COPY_VALUE(&value_, &v.value_);
        }
        // 构造：移动
        value_t(value_t<true>&& v) {
            // 不要在 PHP 引用类型中使用移动构造
            assert(zval_get_type(&value_) != TYPE_REFERENCE); 
            ZVAL_COPY_VALUE(&value_, &v.value_);
            ZVAL_UNDEF(&v.value_);
        }
        // 构造：移动
        value_t(value_t<false>&& v) {
            // 不要在 PHP 引用类型中使用移动构造
            assert(zval_get_type(&value_) != TYPE_REFERENCE); 
            ZVAL_COPY_VALUE(&value_, &v.value_);
            ZVAL_UNDEF(&v.value_);
        }
        // 构建：未定义
        value_t() {
            ZVAL_UNDEF(&value_);
        }
        // 构造：NULL
        value_t(std::nullptr_t) {
            ZVAL_NULL(&value_);
        }
        // 构造：布尔
        value_t(bool b) {
            if(b) ZVAL_TRUE(&value_);
            else ZVAL_FALSE(&value_);
        }
        // 构造：整数
        value_t(int i) {
            ZVAL_LONG(&value_, i);
        }
        // 构造：整数
        value_t(std::int64_t i) {
            ZVAL_LONG(&value_, i);
        }
        // 构造：字符串
        value_t(const char* str) {
            ZVAL_NEW_STR(&value_, zend_string_init(str, std::strlen(str), 0));
        }
        // 构造：字符串
        value_t(std::string_view str) {
            ZVAL_NEW_STR(&value_, zend_string_init(str.data(), str.size(), 0));
        }
        //构造：字符串
        value_t(std::string str) {
            ZVAL_NEW_STR(&value_, zend_string_init(str.data(), str.size(), 0));
        }
        // 构造：字符串
        value_t(zend_string* str, bool add_ref = ReleaseAfterUse) {
            ZVAL_STR(&value_, str);
            if(add_ref) addref();
        }
        // 构造：数组
        value_t(zend_array* arr, bool add_ref = ReleaseAfterUse) {
            ZVAL_ARR(&value_, arr);
            if(add_ref) addref();
        }
        // 构造：对象
        value_t(zend_object* obj, bool add_ref = ReleaseAfterUse) {
            ZVAL_OBJ(&value_, obj);
            if(add_ref) addref();
        }
        // 类型判定
        inline bool is(type_code_t code) const {
            zend_uchar t = zval_get_type(&value_);
            return t == code
                || code == FAKE_CALLABLE && zend_is_callable(&value_, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr)
                || code == FAKE_BOOLEAN && (t == TYPE_TRUE || t == TYPE_FALSE);
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
        // 在需要时进行释放
        ~value_t() {
            if constexpr (ReleaseAfterUse) zval_ptr_dtor(&value_);
        }
        // 类型数据
        inline operator zval*() const {
            return &value_;
        }
        // 类型数据：布尔
        operator bool() const {
            assert(zval_get_type(&value_) == TYPE_TRUE || zval_get_type(&value_) == TYPE_FALSE);
            return zval_get_type(&value_) == TYPE_TRUE;
        }
        // 类型数据：整数
        operator std::int64_t() const {
            assert(zval_get_type(&value_) == TYPE_INTEGER);
            return Z_LVAL(value_);
        }
        // 类型数据：浮点
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
        // 类型数据：数组
        operator array*() const {
            assert(zval_get_type(&value_) == TYPE_ARRAY);
            return reinterpret_cast<array*>(Z_ARR(value_));
        }
        // 类型数据：数组
        value_t<true>& operator [](int idx) const {
            return as<array>()->operator[](idx);
        }
        // 类型数据：（关联）数组
        value_t<true>& operator [](const value_t<true>& idx) const {
            return as<array>()->operator[](idx);
        }
        // 数据类型：可调用
        value_t<true> operator ()() const {
            return as<callable>()->operator()();
        }
        // 数据类型：可调用
        value_t<true> operator ()(std::vector<value_t<true>> argv) const {
            return as<callable>()->operator()(argv);
        }
        // 数据类型：对象
        operator zend_object*() const {
            assert(zval_get_type(&value_) == TYPE_OBJECT);
            return Z_OBJ(value_);
        }
        // 数据类型：对象
        operator object*() const {
            assert(zval_get_type(&value_) == TYPE_OBJECT);
            return reinterpret_cast<object*>(Z_OBJ(value_));
        }
        // 数据类型：对象
        object* operator ->() const {
            assert(zval_get_type(&value_) == TYPE_OBJECT);
            return reinterpret_cast<object*>(Z_OBJ(value_));
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
        // 访问内部类型 (方法)
        template <class T, typename ENABLE = decltype(T::TYPE_CODE)>
        T* as() const noexcept {
            assert(zval_get_type(&value_) == T::TYPE_CODE);
            return reinterpret_cast<T*>(Z_PTR(value_));
        }
        // 特化 callable 实现
        template <>
        callable* as() const noexcept;
    };
    using value = value_t<true>; // 
}

// 嵌入哈析函数，方便容器使用
namespace std {
    template<> struct hash<php::value> {
        typedef php::value argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept {
            assert(s.is(php::TYPE_STRING));
            return s.as<php::string>()->hashcode();
        }
    };
}

#endif // LIBPHPEXT_VALUE_H
