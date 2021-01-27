#include "array.h"
#include "array_iterator.h"
#include "exception.h"
#include "value.h"
#include "runtime.h"

namespace php {
    // 创建数组
    value array::create(std::size_t size) {
        value v;
        ZVAL_NEW_ARR(v);
        // 空数组初始的引用计数标记为 2 故在进行任何更新型操作时会进行分离复制
        // if(size == 0) return const_cast<zend_array*>(&zend_empty_array);
		zend_hash_init(v, size, nullptr, ZVAL_PTR_DTOR, 0);
        return v;
    }
    
    // 归并
    void array::merge(zend_array* target, zend_array* source, bool recursive) {
        if(recursive) php_array_merge_recursive(target, source);
        else php_array_merge(target, source);
    }
    // 数组项查找
    zval* array::find(const zend_array* a, const zval* k) {
        zval* v;
        switch (zval_get_type(k)) {
        case TYPE_STRING: // 内部（可能）存在 hash 缓存
            v = zend_hash_find(a, Z_STR_P(k));
        case TYPE_INTEGER:
            v = zend_hash_index_find(a, Z_LVAL_P(k));
        default:
            throw type_error("Only Int or String can be used as Array indices", -1); // 不支持的类型
        }
        return v;
    }
    // 设置指定项（多级 a.b.c 键）
    void array::set(zend_array* array, std::string key, zval* v) {
        auto p = key.find_last_of('.');
        std::string last_key = key.substr(p+1);
        std::istringstream ss {key.substr(0, p)};
        zval* tmp;
        while(std::getline(ss, key, '.')) {
            tmp = zend_hash_str_find(array, key.data(), key.size());
            if(tmp == nullptr) {
                ZVAL_NEW_ARR(tmp);
                zend_symtable_str_update(array, key.data(), key.size(), tmp);
                array = Z_ARR_P(tmp);
            }
            else if(zval_get_type(tmp) == IS_ARRAY)
                array = Z_ARR_P(tmp);
            else
                throw php::type_error("Cannot set key of types other than Array");
        }
        zend_symtable_str_update(array, last_key.data(), last_key.size(), v);
        zval_add_ref(v);
    }
    // 读取指定项（多级 a.b.c 键）
    zval* array::get(zend_array* array, std::string key) {
        std::stringstream ss (key);
        zval* tmp;
        while(std::getline(ss, key, '.')) {
            tmp = zend_hash_str_find(array, key.data(), key.size());
            if(tmp != nullptr && zval_get_type(tmp) == IS_ARRAY)
                array = Z_ARR_P(tmp);
            else
                return nullptr;
        }
        return tmp;
    }
    // 数组元素访问（引用，可能返回 undefined 值）
    value& array::get(int idx) const {
        zval* v = zend_hash_index_find(this, idx);
        return v ? *reinterpret_cast<value*>(v) : runtime::undefined_value;
    }
    // 数组元素访问（引用，可能返回 undefined 值）
    value& array::get(std::uint64_t idx) const {
        zval* v = zend_hash_index_find(this, idx);
        return v ? *reinterpret_cast<value*>(v) : runtime::undefined_value;
    }
    value& array::get(const char* key) const {
        return get(std::string_view(key));
    }
    // 数组元素访问（引用，可能返回 undefined 值）
    value& array::get(std::string_view key) const {
        zval* v = zend_hash_str_find(this, key.data(), key.size());
        return v ? *reinterpret_cast<value*>(v) : runtime::undefined_value;
    }
    // 数组元素访问（引用，可能返回 undefined 值）
    value& array::get(const value& key) const {
        zval* v = find(this, key);
        return v ? *reinterpret_cast<value*>(v): runtime::undefined_value;
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
            v = zend_hash_index_add_empty_element(this, idx);
        return *reinterpret_cast<value*>(v);
    }
    // 数组元素访问（不存在时创建）
    value& array::operator [](std::string_view key) {
        zval* v = zend_hash_str_find(this, key.data(), key.size());
        if(v == nullptr) // 不存在 -> 创建
            v = zend_hash_str_add_empty_element(this, key.data(), key.size());
        return *reinterpret_cast<value*>(v);
    }
    // 数组元素访问（不存在时创建）
    value& array::operator [](const value& key) { // array_set_zval_key
        zval* v = find(this, key);
        if(v == nullptr) // 不存在 -> 创建
            v = key.is(TYPE_STRING)
                    ? zend_hash_add_empty_element(this, key)
                    : zend_hash_index_add_empty_element(this, key);
        return *reinterpret_cast<value*>(v);
    }
    // 检查指定 KEY 是否存在
    bool array::contains(const value& key) const {
        return find(this, key) != nullptr;
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
        return array_iterator {this, this->nNumUsed};
    }
    // 遍历：倒序（起点）
    array_reverse_iterator array::rbegin() const noexcept {
        HashPosition pos;
        zend_hash_internal_pointer_end_ex(const_cast<array*>(this), &pos);
        return array_reverse_iterator(this, pos);
    }
    // 遍历：倒序（终点）
    array_reverse_iterator array::rend() const noexcept {
        return array_reverse_iterator {this, this->nNumUsed};
    }
}