#ifndef LIBPHPEXT_ARRAY_H
#define LIBPHPEXT_ARRAY_H

#include "vendor.h"
#include "type_code.h"

namespace php {
    class value;
    // 数组正序迭代器
    class array_iterator;
    class array_reverse_iterator;
    // 数组
    class array: public zend_array {
    public:
        static constexpr type_code_t TYPE_CODE = TYPE_ARRAY;
        // 创建数组
        static value create(std::size_t size = 0);
        // 归并
        static void merge(zend_array* target, zend_array* source, bool recursive = false);
        // 数组项查找
        static value& find(const zend_array* a, const value& k);
        // 元素个数
        inline std::size_t size() {
            return nNumOfElements;
        }
        // 数组元素访问（引用，可能返回 undefined 值）
        value get(int idx) const;
        // 数组元素访问（引用，可能返回 undefined 值）
        value get(std::uint64_t idx) const;
        // 数组元素访问（引用，可能返回 undefined 值）
        value get(const char* key) const;
        // 数组元素访问（引用，可能返回 undefined 值）
        value get(std::string_view key) const;
        // 数组元素访问（引用，可能返回 undefined 值）
        value get(const value& key) const;
        // 数组元素设置
        void set(int idx, const value& val) { return set(static_cast<std::uint64_t>(idx), val); }
        // 数组元素设置
        void set(std::uint64_t idx, const value& val);
        // 数组元素设置
        void set(const char* key, const value& val) {
            set(std::string_view(key), val);
        }
        // 数组元素设置
        void set(std::string_view key, const value& val);
        // 数组元素设置
        void set(const value& key, const value& val);
        // 数组元素访问（不存在时创建）
        value& operator [](int idx) const { return operator[](static_cast<std::uint64_t>(idx)); }
        // 数组元素访问（不存在时创建）
        value& operator [](std::uint64_t idx);
        // 数组元素访问（不存在时创建）
        value& operator [](const char* key) {
            return operator [](std::string_view(key));
        }
        // 数组元素访问（不存在时创建）
        value& operator [](std::string_view key);
        // 数组元素访问（不存在时创建）
        value& operator [](const value& key);
        // 追加元素
        void append(const value& val);
        // 检查指定 IDX 是否存在
        bool contains(int idx) const {
            return contains(static_cast<std::uint64_t>(idx));
        }
        // 检查指定 IDX 是否存在
        bool contains(std::uint64_t idx) const {
            return zend_hash_index_exists(this, idx);
        }
        bool contains(const char* key) const {
            return contains(std::string_view(key));
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
