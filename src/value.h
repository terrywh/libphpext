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
        value_t(uint u) {
            ZVAL_LONG(&value_, u);
        }
        // 构造：整数
        value_t(std::int64_t i) {
            ZVAL_LONG(&value_, i);
        }
        value_t(std::uint64_t u) {
            ZVAL_LONG(&value_, u);
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
        // 类型编号
        inline type_code_t type_code() const {
            return static_cast<type_code_t>(zval_get_type(&value_));
        }
        // 类型判定
        inline bool is(type_code_t code) const {
            zend_uchar t = zval_get_type(&value_);
            return t == code
                || code == FAKE_CALLABLE && zend_is_callable(&value_, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr)
                || code == FAKE_BOOLEAN && (t == TYPE_TRUE || t == TYPE_FALSE);
        }
        // 引用计数
        int addref() const {
            assert(Z_REFCOUNTED(value_));
            return Z_ADDREF(value_);
        }
        // 引用计数（不应减为零）
        int delref() const {
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
        // 注意 operator bool 用于条件判定，等价于 !empty()
        // 类型数据：布尔
        bool boolean() const {
            assert(zval_get_type(&value_) == TYPE_TRUE || zval_get_type(&value_) == TYPE_FALSE);
            return zval_get_type(&value_) == TYPE_TRUE;
        }
        // 类型数据：整数
        operator std::int64_t() const {
            assert(zval_get_type(&value_) == TYPE_INTEGER);
            return Z_LVAL(value_);
        }
        operator std::uint64_t() const {
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
        // 赋值
        value_t& operator =(const value_t& v) {
            zval_ptr_dtor(&value_);
            ZVAL_COPY(&value_, &v.value_);
            return *this;
        }
        // 赋值
        value_t& operator =(value_t&& v) {
            zval_ptr_dtor(&value_);
            ZVAL_COPY_VALUE(&value_, &v.value_);
            ZVAL_UNDEF(&v.value_);
            return *this;
        }
        // 运算符：相等
        bool operator ==(const value_t& v) const {
            value_t r;
            return is_equal_function(&r.value_, &value_, &v.value_);
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
        template <class T, typename = std::enable_if_t<T::TYPE_CODE != FAKE_CALLABLE>>
        T* as() const noexcept {
            assert(zval_get_type(&value_) == T::TYPE_CODE);
            return reinterpret_cast<T*>(Z_PTR(value_));
        }
        // 特化 callable 实现
        template <class T, typename = std::enable_if_t<T::TYPE_CODE == FAKE_CALLABLE>>
        callable* as() const noexcept {
            assert(zend_is_callable(&value_, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr));
            return reinterpret_cast<callable*>(&value_);
        }
        // 空判定
        bool empty() const {
            switch(zval_get_type(&value_)) {
            case TYPE_UNDEFINED:
            case TYPE_NULL:
            case TYPE_FALSE:
                return true;
            case TYPE_LONG:
            case TYPE_DOUBLE:
                return Z_LVAL(value_) == 0;
            case TYPE_STRING:
                return Z_STRLEN(value_) == 0;
            case TYPE_ARRAY:
                return Z_ARRVAL(value_)->nNumOfElements == 0;
            case TYPE_REFERENCE:
                return reinterpret_cast<value*>(Z_REFVAL(value_))->empty();
            case TYPE_RESOURCE:
                return false; // 资源型 非空
            default:
                return true; // 一切皆空
            }
        }
        // 空判定
        operator bool() const {
            return !empty();
        }
        // 长度大小（仅对字符串、数组有效）
        std::size_t size() const {
            switch(zval_get_type(&value_)) {
            case TYPE_STRING:
                return Z_STRLEN(value_);
            case TYPE_ARRAY:
                return Z_ARRVAL(value_)->nNumOfElements;
            case TYPE_REFERENCE:
                return reinterpret_cast<value*>(Z_REFVAL(value_))->size();
            default:
                return 0;
            }
        }

#define DECLARE_CREASE_THIS(OPR) value_t& operator OPR() { \
    assert(zval_get_type(&value_) == TYPE_INTEGER);        \
    OPR Z_LVAL(value_);                                    \
    return *this;                                          \
}
        DECLARE_CREASE_THIS(++);
        DECLARE_CREASE_THIS(--);
#undef DECLARE_CREASE_THIS

#define DECLARE_CREASE_THAT(OPR) value_t operator OPR(int) { \
    assert(zval_get_type(&value_) == TYPE_INTEGER);          \
    value_t y {*this};                                       \
    OPR y;                                                   \
    return y;                                                \
}
        DECLARE_CREASE_THAT(++);
        DECLARE_CREASE_THAT(--);
#undef DELCARE_CREASE_THAT

#define DECLARE_OPERATOR_THIS(CODE, TYPE, OPR)          \
    value_t& operator OPR(TYPE x) {                     \
        assert(zval_get_type(&value_) == CODE);         \
        Z_LVAL(value_) OPR x;                           \
        return *this;                                   \
    }
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, int, +=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, int, -=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, int, *=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, int, /=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, std::int64_t, +=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, std::int64_t, -=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, std::int64_t, *=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, std::int64_t, /=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, float, +=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, float, -=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, float, *=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, float, /=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, double, +=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, double, -=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, double, *=);
        DECLARE_OPERATOR_THIS(TYPE_INTEGER, double, /=);
#undef DECLARE_OPERATOR_THIS

#define DECLARE_OPERATOR_THAT(CODE, TYPE, OPR, AOPR)    \
    value_t operator OPR(TYPE x) {                      \
        assert(zval_get_type(&value_) == CODE);         \
        value_t y {*this};                              \
        return y AOPR x;                                \
    }
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, int, +, +=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, int, -, -=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, int, *, *=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, int, /, /=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, std::int64_t, +, +=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, std::int64_t, -, -=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, std::int64_t, *, *=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, std::int64_t, /, /=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, float, +, +=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, float, -, -=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, float, *, *=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, float, /, /=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, double, +, +=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, double, -, -=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, double, *, *=)
        DECLARE_OPERATOR_THAT(TYPE_INTEGER, double, /, /=)
#undef DECLARE_OPERATOR_THAT
    };
    // 自动释放类型
    using value = value_t<true>;
    using refer = value_t<false>;
    // 自动序列化
    template <bool RAU>
    std::ostream& operator << (std::ostream& os, const value_t<RAU>& data) {
        if(!data.is(TYPE_STRING)) {
            smart_str buffer {nullptr, false};
            php_json_encode(&buffer, data, PHP_JSON_UNESCAPED_UNICODE);
            os.write(buffer.s->val, buffer.s->len);
            smart_str_free_ex(&buffer, false);
        }
        else {
            std::string_view sv = data;
            os.write(sv.data(), sv.size());
        }
        return os;
    }
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
