#ifndef LIBPHPEXT_VALUE_BASIC_H
#define LIBPHPEXT_VALUE_BASIC_H

#include "vendor.h"
#include "type.h"
#include "string.h"
#include "object.h"
#include "array.h"
#include "callable.h"
#include "object.h"

namespace php {

    // PHP 变量（功能包裹）
    template <class R, typename T,
            typename = decltype(T::pointer( reinterpret_cast<R*>(0) ))>
    class basic_value {
    private:
        inline zval* ptr(const basic_value* self) const { return T::pointer(reinterpret_cast<const R*>( self )); }
        inline zval* ptr(const basic_value& self) const { return T::pointer(reinterpret_cast<const R*>(&self )); }
    public:
         // 类型编号
        inline type_code type() const {
            return static_cast<type_code>(zval_get_type(ptr(this)));
        }
        // 类型判定
        inline bool is(type_code code) const {
            zend_uchar t = zval_get_type(ptr(this));
            return t == code
                || code == FAKE_CALLABLE && zend_is_callable(ptr(this), IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr)
                || code == FAKE_BOOLEAN && (t == TYPE_TRUE || t == TYPE_FALSE);
        }
        // 实例判定
        inline bool of(zend_class_entry* ce) const {
            return zval_get_type(ptr(this)) == TYPE_OBJECT && instanceof_function(Z_OBJCE_P(ptr(this)), ce);
        }
        // 引用计数
        int addref() const {
            assert(Z_REFCOUNTED_P(ptr(this)));
            return Z_ADDREF_P(ptr(this));
        }
        // 引用计数（不应减为零）
        int delref() const {
            assert(Z_REFCOUNTED_P(ptr(this)));
            return Z_DELREF_P(ptr(this));
        }
        // 构造：复制
        basic_value(const basic_value& v) {
            ZVAL_COPY(ptr(this), v.is(FAKE_REFERENCE) ? Z_REFVAL_P(ptr(v)) : ptr(v));
        }
        // 构造：移动
        basic_value(basic_value&& v) {
            // 不要在 PHP 引用类型中使用移动构造
            assert(zval_get_type(ptr(this)) != FAKE_REFERENCE);
            ZVAL_COPY_VALUE(ptr(this), ptr(v));
            ZVAL_UNDEF(ptr(v));
        }
        // 构建：未定义
        basic_value() = default;
        // 构造：NULL
        basic_value(std::nullptr_t) {
            ZVAL_NULL(ptr(this));
        }
        // 构造：布尔
        basic_value(bool b) {
            if(b) ZVAL_TRUE(ptr(this));
            else ZVAL_FALSE(ptr(this));
        }
        // 构造：整数
        basic_value(int i) {
            ZVAL_LONG(ptr(this), i);
        }
        // 构造：整数
        basic_value(uint u) {
            ZVAL_LONG(ptr(this), u);
        }
        // 构造：整数
        basic_value(std::int64_t i) {
            ZVAL_LONG(ptr(this), i);
        }
        basic_value(std::uint64_t u) {
            ZVAL_LONG(ptr(this), u);
        }
        basic_value(const char* str)
        : basic_value(std::string_view(str)) {}
        // 构造：字符串
        basic_value(std::string_view str) {
            ZVAL_NEW_STR(ptr(this), zend_string_init(str.data(), str.size(), 0));
        }
        //构造：字符串
        basic_value(std::string str) {
            ZVAL_NEW_STR(ptr(this), zend_string_init(str.data(), str.size(), 0));
        }
        // 构造：字符串
        explicit basic_value(zend_string* str, bool add_reference = true) {
            ZVAL_STR(ptr(this), str);
            if(add_reference) addref();
        }
        // 构造：数组
        explicit basic_value(zend_array* arr, bool add_reference = true) {
            ZVAL_ARR(ptr(this), arr);
            if(add_reference) addref();
        }
        // 构造：对象
        explicit basic_value(zend_object* obj, bool add_reference = true) {
            ZVAL_OBJ(ptr(this), obj);
            if(add_reference) addref();
        }
        explicit basic_value(zval* val, bool add_reference = true) {
            if(add_reference) ZVAL_COPY(ptr(this), val);
            else ZVAL_COPY_VALUE(ptr(this), val);
        }
        operator zval*() const {
            return ptr(this);
        }
        // 转换为 bool 类型数据
        // 注意: operator bool 用于条件判定，等价于 !empty()
        // 类型数据：布尔
        bool boolean() const {
            assert(zval_get_type(ptr(this)) == TYPE_TRUE || zval_get_type(ptr(this)) == TYPE_FALSE);
            return zval_get_type(ptr(this)) == TYPE_TRUE;
        }
        // 类型数据：整数
        operator int() const {
            assert(zval_get_type(ptr(this)) == TYPE_INTEGER);
            return Z_LVAL_P(ptr(this));
        }
        // 类型数据：整数
        operator std::int64_t() const {
            assert(zval_get_type(ptr(this)) == TYPE_INTEGER);
            return Z_LVAL_P(ptr(this));
        }
        operator std::uint64_t() const {
            assert(zval_get_type(ptr(this)) == TYPE_INTEGER);
            return Z_LVAL_P(ptr(this));
        }
        // 类型数据：浮点
        operator double() const {
            assert(zval_get_type(ptr(this)) == TYPE_FLOAT);
            return Z_DVAL_P(ptr(this));
        }
        // 类型数据：字符串
        operator zend_string*() const {
            assert(zval_get_type(ptr(this)) == TYPE_STRING);
            return Z_STR_P(ptr(this));
        }
        operator string&() const {
            assert(zval_get_type(ptr(this)) == TYPE_STRING);
            return *reinterpret_cast<string*>(Z_STR_P(ptr(this)));
        }
        // 类型数据：字符串
        operator std::string_view() const { // 返回 string_view 可直接访问内存
            assert(zval_get_type(ptr(this)) == TYPE_STRING);
            return {Z_STRVAL_P(ptr(this)), Z_STRLEN_P(ptr(this))};
        }
        // 类型数据：字符串
        operator std::string() const {
            assert(zval_get_type(ptr(this)) == TYPE_STRING);
            return {Z_STRVAL_P(ptr(this)), Z_STRLEN_P(ptr(this))};
        }
        // 类型数据：数组
        operator zend_array*() const {
            assert(zval_get_type(ptr(this)) == TYPE_ARRAY);
            return Z_ARR_P(ptr(this));
        }
        // 类型数据：数组
        operator array&() const {
            assert(zval_get_type(ptr(this)) == TYPE_ARRAY);
            return *reinterpret_cast<array*>(Z_ARR_P(ptr(this)));
        }
        // 类型数据：数组
        auto& operator [](int idx) const {
            return as<array>()->operator[](idx);
        }
        // 类型数据：（关联）数组
        auto& operator [](const basic_value& idx) const {
            return as<array>()->operator[](idx);
        }
        // 数据类型：可调用
        auto operator ()() const {
            return as<callable>().operator()();
        }
        // 数据类型：可调用
        auto operator ()(std::vector<R> argv) const {
            return as<callable>().operator()(argv);
        }
        // 数据类型：对象
        operator zend_object*() const {
            assert(zval_get_type(ptr(this)) == TYPE_OBJECT);
            return Z_OBJ_P(ptr(this));
        }
        // 数据类型：对象
        operator object&() const {
            assert(zval_get_type(ptr(this)) == TYPE_OBJECT);
            return *reinterpret_cast<object*>(Z_OBJ_P(ptr(this)));
        }
        // 数据类型：对象
        object* operator ->() const {
            assert(zval_get_type(ptr(this)) == TYPE_OBJECT);
            return reinterpret_cast<object*>(Z_OBJ_P(ptr(this)));
        }
        // 访问内部类型 (方法)
        template <class S, typename = std::enable_if_t<S::TYPE_CODE != FAKE_CALLABLE>>
        S& as() const noexcept {
            assert(zval_get_type(ptr(this)) == S::TYPE_CODE);
            return *reinterpret_cast<S*>(Z_PTR_P(ptr(this)));
        }
        // 特化 callable 实现
        template <class S, typename = std::enable_if_t<S::TYPE_CODE == FAKE_CALLABLE>>
        callable& as() const noexcept {
            assert(zend_is_callable(ptr(this), IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr));
            return *reinterpret_cast<callable*>(ptr(this));
        }
        // 空判定
        bool empty() const {
            type_code code = type();
        RECHECK_EMPTY:
            switch(code) {
            case TYPE_UNDEFINED:
            case TYPE_NULL:
            case TYPE_FALSE:
                return true;
            case TYPE_LONG:
            case TYPE_DOUBLE:
                return Z_LVAL_P(ptr(this)) == 0;
            case TYPE_STRING:
                return Z_STRLEN_P(ptr(this)) == 0;
            case TYPE_ARRAY:
                return Z_ARRVAL_P(ptr(this))->nNumOfElements == 0;
            case FAKE_REFERENCE:
                code = static_cast<type_code>(zval_get_type(Z_REFVAL_P(ptr(this))));
                goto RECHECK_EMPTY;
            case TYPE_TRUE:
            case TYPE_RESOURCE:
                return false; // 资源非空
            default:
                return true;  // 万般皆空
            }
        }
        // 空判定
        operator bool() const {
            return !empty();
        }
        // 长度大小（仅对字符串、数组有效）
        std::size_t size() const {
            zval* self = ptr(this);
            type_code code = type();

        RECALC_SIZE:
            switch(code) {
            case TYPE_STRING:
                return Z_STRLEN_P(self);
            case TYPE_ARRAY:
                return Z_ARRVAL_P(self)->nNumOfElements;
            case FAKE_REFERENCE:
                self = Z_REFVAL_P(self);
                code = static_cast<type_code>(zval_get_type(self));
                goto RECALC_SIZE;
            default:
                return 0;
            }
        }
        // 赋值
        basic_value& operator =(const basic_value& v) {
            zval_ptr_dtor(ptr(this));
            ZVAL_COPY(ptr(this), ptr(v));
            return *this;
        }
        // 赋值
        basic_value& operator =(basic_value&& v) {
            zval_ptr_dtor(ptr(this));
            ZVAL_COPY_VALUE(ptr(this), ptr(v));
            ZVAL_UNDEF(ptr(v));
            return *this;
        }
        // 运算符：相等
        bool operator ==(const basic_value& v) const {
            return zend_compare(ptr(this), ptr(v)) == 0;
        }
        // 运算符：相等
        bool operator ==(std::string_view v) const {
            return zval_get_type(ptr(this)) == TYPE_STRING &&
                zend_binary_strcmp(Z_STRVAL_P(ptr(this)), Z_STRLEN_P(ptr(this)), v.data(), v.size()) == 0;
        }
        // 运算符：比较
        bool operator <(const basic_value& v) const {
            return zend_compare(ptr(this), ptr(v)) < 0;
        }
        // 运算符：比较
        bool operator <(int v) const {
            assert(zval_get_type(ptr(this)) == TYPE_INTEGER);
            return Z_LVAL_P(ptr(this)) > v;
        }
        // 运算符：比较
        bool operator <(std::int64_t v) const {
            assert(zval_get_type(ptr(this)) == TYPE_INTEGER);
            return Z_LVAL_P(ptr(this)) > v;
        }
        // 运算符：比较
        bool operator <(std::string_view v) const {
            assert(zval_get_type(ptr(this)) == TYPE_STRING);
            return zend_binary_strcmp(Z_STRVAL_P(ptr(this)), Z_STRLEN_P(ptr(this)), v.data(), v.size()) < 0;
        }
#define DECLARE_CREASE_THIS(OPR) R& operator OPR() {   \
    assert(zval_get_type(ptr(this)) == TYPE_INTEGER);  \
    OPR Z_LVAL_P(ptr(this));                           \
    return *reinterpret_cast<R*>(this);                \
}
        DECLARE_CREASE_THIS(++);
        DECLARE_CREASE_THIS(--);
#undef DECLARE_CREASE_THIS

#define DECLARE_CREASE_THAT(OPR) R operator OPR(int) { \
    assert(zval_get_type(ptr(this)) == TYPE_INTEGER);  \
    R y {*this};                                       \
    OPR y;                                             \
    return *reinterpret_cast<R*>(this);                \
}
        DECLARE_CREASE_THAT(++);
        DECLARE_CREASE_THAT(--);
#undef DELCARE_CREASE_THAT

#define DECLARE_OPERATOR_THIS(CODE, TYPE, OPR)     \
    R& operator OPR(TYPE x) {                      \
        assert(zval_get_type(ptr(this)) == CODE);  \
        Z_LVAL_P(ptr(this)) OPR x;                 \
        return *reinterpret_cast<R*>(this);        \
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

#define DECLARE_OPERATOR_THAT(CODE, TYPE, OPR, AOPR) \
    R operator OPR(TYPE x) {                         \
        assert(zval_get_type(ptr(this)) == CODE);    \
        R y {*this};                                 \
        return y AOPR x;                             \
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

#define OVERRIDE_IMPLICIT_DECLARATION(CLASS_NAME)                  \
        CLASS_NAME(const CLASS_NAME& v): basic_value(v) {}         \
        CLASS_NAME(CLASS_NAME&& v): basic_value(std::move(v)) {}   \
        CLASS_NAME& operator =(const CLASS_NAME& v) {              \
            basic_value::operator=(v); return *this; }             \
        CLASS_NAME& operator =(CLASS_NAME&& v) {                   \
            basic_value::operator=(std::move(v));                  \
            return *this; }
    // 自动序列化
    template <class R, typename T>
    std::ostream& operator << (std::ostream& os, const basic_value<R, T>& data) {
        if(!data.is(TYPE_STRING)) {
            smart_str buffer {nullptr, false};
            php_json_encode(&buffer, data, PHP_JSON_UNESCAPED_UNICODE);
            if(buffer.s) os.write(buffer.s->val, buffer.s->len);
            smart_str_free_ex(&buffer, false);
        }
        else {
            std::string_view sv = data;
            os.write(sv.data(), sv.size());
        }
        return os;
    }

    class property_value;
    class property_refer;
    class parameters;

    class value;
    struct value_traits {
        static zval* pointer(const value* v);
    };
    // PHP 变量（自动释放）
    class value: public basic_value<value, value_traits> {
    private:
        mutable zval value_;
    public:
        // 构建：未定义
        value() {
            ZVAL_UNDEF(&value_);
        }
        ~value() {
            zval_ptr_dtor(&value_);
        }
        using basic_value::basic_value;
        OVERRIDE_IMPLICIT_DECLARATION(value);
        // 构造回调
        value(std::function<value (parameters& params)> fn);
        // 赋值
        template <class T, typename = typename T::value_traits>
        value& operator =(const T& v) {
            zval_ptr_dtor(value_traits::pointer(this));
            ZVAL_COPY(value_traits::pointer(this), T::value_traits::pointer(&v));
            return *this;
        }
        // 数组分离 (用于支持 COW 写时复制)
        basic_value& operator *() {
            SEPARATE_ARRAY(&value_);
            return *this;
        }
        friend struct value_traits;
    };
    // 确认包裹大小一致（可以进行引用）
    static_assert(sizeof(value) == sizeof(zval));
}

// 嵌入哈析函数，方便容器使用
namespace std {
    template<> struct hash<php::value> {
        typedef php::value argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept {
            assert(s.is(php::TYPE_STRING));
            return s.as<php::string>().hashcode();
        }
    };
}

#endif //LIBPHPEXT_VALUE_BASIC_H
