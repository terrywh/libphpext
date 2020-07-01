#include "array.h"
#include "value.h"
#include "array_iterator.h"
#include "env.h"

namespace php {
    static zval* find(const zend_array* a, const zval* k) {
        switch (zval_get_type(k)) {
        case TYPE_STRING: // 内部（可能）存在 hash 缓存
            return zend_hash_find(a, Z_STR_P(k));
        case TYPE_INTEGER:
            return zend_hash_index_find(a, Z_LVAL_P(k));
        default:
            throw type_error("Only Int or String can be used as Array indices", -1); // 不支持的类型
        }
    }
    // 数组元素访问（引用，可能返回 undefined 值）
    value array::get(int idx) const {
        zval* v = zend_hash_index_find(this, idx);
        return v ? *reinterpret_cast<value*>(v) : value();
    }
    // 数组元素访问（引用，可能返回 undefined 值）
    value array::get(std::uint64_t idx) const {
        zval* v = zend_hash_index_find(this, idx);
        return v ? *reinterpret_cast<value*>(v) : value();
    }
    value array::get(const char* key) const {
        return get(std::string_view(key));
    }
    // 数组元素访问（引用，可能返回 undefined 值）
    value array::get(std::string_view key) const {
        zval* v = zend_hash_str_find(this, key.data(), key.size());
        return v ? *reinterpret_cast<value*>(v) : value();
    }
    // 数组元素访问（引用，可能返回 undefined 值）
    value array::get(const value& key) const {
        zval* v = find(this, key);
        return v ? *reinterpret_cast<value*>(v) : value();
    }
    // 数组元素设置
    void array::set(std::uint64_t idx, const value& val) {
        val.addref(); // 放入数组，增加引用
        zend_hash_index_update(this, idx, val); // add_index_zval
    }
    // 数组元素设置
    void array::set(std::string_view key, const value& val) {
        val.addref();
        zend_symtable_str_update(this, key.data(), key.size(), val); // add_asoc_zval
    }
    // 数组元素设置
    void array::set(const value& key, const value& val) {
        val.addref();
        switch(zval_get_type(key)) {
        case TYPE_STRING:
            zend_symtable_update(this, key, val);
            break;
        case TYPE_INTEGER:
            zend_hash_index_update(this, key, val);
            break;
        default:
            throw type_error("Only Int or String can be used as Array indices", -1); // 不支持的类型
        }
    }
    // 数组元素访问（不存在时创建）
    value& array::operator [](std::uint64_t idx) {
        zval* v = zend_hash_index_find(this, idx);
        if(v == nullptr) // 不存在 -> 创建
            v = zend_hash_index_update(this, idx, &EG(uninitialized_zval)); // add_index_zval
        return *reinterpret_cast<value*>(v);
    }
    // 数组元素访问（不存在时创建）
    value& array::operator [](std::string_view key) {
        zval* v = zend_hash_str_find(this, key.data(), key.size());
        if(v == nullptr) // 不存在 -> 创建
            v = zend_symtable_str_update(this, key.data(), key.size(), &EG(uninitialized_zval)); // add_asoc_zval
        return *reinterpret_cast<value*>(v);
    }
    // 数组元素访问（不存在时创建）
    value& array::operator [](const value& key) { // array_set_zval_key
        zval* v = find(this, key);
        if(v == nullptr) // 不存在 -> 创建
            v = key.is(TYPE_STRING)
                    ? zend_symtable_update(this, key, &EG(uninitialized_zval))
                    : zend_hash_index_update(this, key, &EG(uninitialized_zval));
        return *reinterpret_cast<value*>(v);
    }
    // 检查指定 KEY 是否存在
    bool array::contains(const value& key) const {
        if(key.is(TYPE_STRING)) // 内部（可能）存在 hash 缓存
            return zend_hash_exists(this, key);
        else if(key.is(TYPE_INTEGER))
            return zend_hash_index_exists(this, key);
        else throw type_error("Only Int or String can be used as Array indices", -1); // 不支持的类型
    }
    // 追加元素
    void array::append(const value& val) {
        val.addref();
        zend_hash_next_index_insert(this, val); // add_next_index_zval
    }
    // 遍历：正序（起点）
    array_iterator array::begin() const noexcept {
        HashPosition pos;
        zend_hash_internal_pointer_reset_ex(const_cast<array*>(this), &pos);
        return array_iterator(this, pos);
    }
    // 遍历：正序（终点）
    array_iterator array::end() const noexcept {
        static array_iterator e {this, this->nNumUsed};
        return e;
    }
    // 遍历：倒序（起点）
    array_reverse_iterator array::rbegin() const noexcept {
        HashPosition pos;
        zend_hash_internal_pointer_end_ex(const_cast<array*>(this), &pos);
        return array_reverse_iterator(this, pos);
    }
    // 遍历：倒序（终点）
    array_reverse_iterator array::rend() const noexcept {
        return array_reverse_iterator(this, this->nNumUsed);
    }
    
    
}