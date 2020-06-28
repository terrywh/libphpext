#include "array.h"
#include "value.h"
#include "array_iterator.h"

namespace php {
    // 数组类型
    type_code_t array::TYPE_CODE = TYPE_ARRAY;
    // 数组元素访问
    value& array::operator [](const value& key) const {
        if(key.is(TYPE_STRING)) // 内部（可能）存在 hash 缓存
            return *reinterpret_cast<value*>(zend_hash_find(this, key));
        else if(key.is(TYPE_INTEGER)) return operator[](
            static_cast<std::uint64_t>(static_cast<std::int64_t>(key)));
        else throw type_error("Only int or string can be used as Array indices", -1); // 不支持的类型
    }
    // 检查指定 KEY 是否存在
    bool array::contains(const value& key) const {
        if(key.is(TYPE_STRING)) // 内部（可能）存在 hash 缓存
            return zend_hash_exists(this, key);
        else if(key.is(TYPE_INTEGER)) return contains(
            static_cast<std::uint64_t>(static_cast<std::int64_t>(key)));
        else throw type_error("Only int or string can be used as Array indices", -1); // 不支持的类型
    }
    // 遍历：正序（起点）
    array_iterator array::begin() const noexcept {
        HashPosition pos;
        zend_hash_internal_pointer_reset_ex(const_cast<array*>(this), &pos);
        return array_iterator(this, pos);
    }
    // 遍历：正序（终点）
    array_iterator array::end() const noexcept {
        static array_iterator e {this, HT_INVALID_IDX};
        return e;
    }
    // 遍历：倒序（起点）
    array_reverse_iterator array::rbegin() const noexcept {

    }
    // 遍历：倒序（终点）
    array_reverse_iterator array::rend() const noexcept {

    }
    
    
}