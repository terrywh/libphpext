#ifndef LIBPHPEXT_PROPERTY_ENTRY_H
#define LIBPHPEXT_PROPERTY_ENTRY_H

#include "vendor.h"
#include "property_refer.h"

namespace php {
    class value;
    // 属性定义
    class property_entry {
    public:
        // 结构定义参考 zend_constant
        zval           value;
        zend_string*    name;
        std::uint32_t      v; // visibility
        zend_string* comment;

        static void do_register(std::vector<property_entry>& props, zend_class_entry* ce);
    private:
        // 属性定义，可选文档注释
        property_entry(std::string_view key, const php::value& val, std::uint32_t v, zend_string* doc);

        friend property_entry property(std::string_view name, const php::value& data);
        friend property_entry static_property(std::string_view name, const php::value& data);
        template <class T>
        friend property_entry property(std::string_view name, const php::value& data, property_refer T::* prop);
    };

    // 定义属性
    inline property_entry property(std::string_view name, const php::value& data) {
        return { name, data, ZEND_ACC_PUBLIC, nullptr};
    }
    // 定义属性
    inline property_entry static_property(std::string_view name, const php::value& data) {
        return { name, data, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC, nullptr };
    }
    // 定义属性（同步属性）
    // @see https://gist.github.com/graphitemaster/494f21190bb2c63c5516
    // 注意：导出到 PHP 的 C++ 类一定可以默认构造（否则无法通用创建对象）
    template <class T>
    // template <class T, typename = std::enable_if_t< std::is_default_constructible<T>::value >>
    property_entry property(std::string_view name, const php::value& data, property_refer T::* prop) {
        std::size_t offset = offset_of(prop);
        // 注意 _:sync_ 作为特殊标记识别同步型属性，用户注释使用此内容可能出现异常
        zend_string* comment = zend_string_init("_:sync_\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 8 + sizeof(std::size_t), true);
        std::memcpy(&comment->val[8], &offset, sizeof(std::size_t));
        return {name, data, ZEND_ACC_PUBLIC, comment};
    }
}

#endif // LIBPHPEXT_PROPERTY_ENTRY_H
