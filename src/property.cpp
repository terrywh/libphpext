#include "property.h"
#include "value.h"

namespace php {
    zval* property_traits::pointer(const property* v) {
        return &v->value_;
    }
    // 构建对象 (读取赋值属性值）
    property::property(zend_object* obj, zend_string* key)
    : obj_(obj), key_(key) {
        zval *p;
        // 参考 zend_read_property 相关代码
        zend_class_entry *ce = EG(fake_scope);
        EG(fake_scope) = obj_->ce;
        p = obj_->handlers->read_property(obj_, key_, BP_VAR_RW, nullptr, &value_);
        EG(fake_scope) = ce;
        // 在进行 __get() 调用时 p 会引用了传入的 rv = &value_ 指针
        if (p != &value_) ZVAL_COPY_VALUE(&value_, p);
    }
    // 转换 value 通用类型
    property::operator value() {
        return value{property_traits::pointer(this) };
    }
    // 属性更新的通用实现，内部已进行引用计数
#define UPDATE_PROPERTY(v) do {                                                     \
        zend_class_entry *ce = EG(fake_scope);                                      \
        EG(fake_scope) = obj_->ce;                                                  \
        obj_->handlers->write_property(obj_, key_, static_cast<zval*>(v), nullptr); \
        EG(fake_scope) = ce;                                                        \
        ZVAL_COPY_VALUE(&value_, static_cast<zval*>(v));                            \
    } while(false)
    // 赋值：复制
    property& property::operator =(const value& v) {
        UPDATE_PROPERTY(v);
        return *this;
    }
    // 运算符：自增（存在缓存）
    property& property::operator ++() {
        assert(is(TYPE_INTEGER));
        ++Z_LVAL(value_);
        UPDATE_PROPERTY(&value_);
        return *this;
    }
    // 运算符：自减（存在缓存）
    property& property::operator --() {
        assert(is(TYPE_INTEGER));
        --Z_LVAL(value_);
        UPDATE_PROPERTY(&value_);
        return *this;
    }
#define DEFINE_INTEGER_OPERATOR(TYPE, OPR)  property& property::operator OPR(TYPE x) { \
    assert(is(TYPE_INTEGER));    \
    Z_LVAL(value_) OPR x;     \
    UPDATE_PROPERTY(&value_); \
    return *this;             \
}
    DEFINE_INTEGER_OPERATOR(int, +=);
    DEFINE_INTEGER_OPERATOR(int, -=);
    DEFINE_INTEGER_OPERATOR(int, *=);
    DEFINE_INTEGER_OPERATOR(int, /=);
    DEFINE_INTEGER_OPERATOR(std::int64_t, +=);
    DEFINE_INTEGER_OPERATOR(std::int64_t, -=);
    DEFINE_INTEGER_OPERATOR(std::int64_t, *=);
    DEFINE_INTEGER_OPERATOR(std::int64_t, /=);
#undef DEFINE_INTEGER_OPERATOR
#define DEFINE_FLOAT_OPERATOR(TYPE, OPR)  property& property::operator OPR(TYPE x) { \
    assert(is(TYPE_FLOAT));    \
    Z_LVAL(value_) OPR x;     \
    UPDATE_PROPERTY(&value_); \
    return *this;             \
}
    DEFINE_FLOAT_OPERATOR(float, +=);
    DEFINE_FLOAT_OPERATOR(float, -=);
    DEFINE_FLOAT_OPERATOR(float, *=);
    DEFINE_FLOAT_OPERATOR(float, /=);
    DEFINE_FLOAT_OPERATOR(double, +=);
    DEFINE_FLOAT_OPERATOR(double, -=);
    DEFINE_FLOAT_OPERATOR(double, *=);
    DEFINE_FLOAT_OPERATOR(double, /=);
#undef DEFINE_FLOAT_OPERATOR
}