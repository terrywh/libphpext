#ifndef LIBPHPEXT_ARRAY_H
#define LIBPHPEXT_ARRAY_H

#include "vendor.h"
#include "type_code.h"
#include "exception.h"

namespace php {
    template <bool RAU>
    class value_t;
    using value = value_t<true>;
    // 数组正序迭代器
    class array_iterator;
    class array_reverse_iterator;
    // 数组
    class array: public zend_array {
    public:
        static type_code_t TYPE_CODE;
        // 元素个数
        inline std::size_t size() {
            return nNumOfElements;
        }
        // 数组元素访问
        value& operator [](int idx) const {
            return operator[](static_cast<std::uint64_t>(idx));
        }
        // 数组元素访问
        value& operator [](std::uint64_t idx) const {
            return *reinterpret_cast<value*>(zend_hash_index_find(this, idx));
        }
        // 数组元素访问
        value& operator [](std::string_view key) const {
            return *reinterpret_cast<value*>(zend_hash_str_find(this, key.data(), key.size()));
        }
        // 数组元素访问
        value& operator [](const value& key) const;
        // 检查指定 IDX 是否存在
        bool contains(int idx) const {
            return contains(static_cast<std::uint64_t>(idx));
        }
        // 检查指定 IDX 是否存在
        bool contains(std::uint64_t idx) const {
            return zend_hash_index_exists(this, idx);
        }
        // 检查指定 KEY 是否存在
        bool contains(std::string_view key) const {
            return zend_hash_str_exists(this, key.data(), key.size());
        }
        // 检查指定 KEY 是否存在
        bool contains(const value& key) const;
        // 遍历：正序（起点）
        array_iterator begin() const noexcept;
        // 遍历：正序（终点）
        array_iterator end() const noexcept;
        // 遍历：倒序（起点）
        array_reverse_iterator rbegin() const noexcept;
        // 遍历：倒序（终点）
        array_reverse_iterator rend() const noexcept;
    };
}

#endif // LIBPHPEXT_ARRAY_H
