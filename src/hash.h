#ifndef LIBPHPEXT_HASH_H
#define LIBPHPEXT_HASH_H

#include "vendor.h"
#include "value.h"

namespace php {
    //
    std::uint32_t crc32(std::string_view data);
    // PHP 内置哈系表对应哈系函数
    inline std::uint64_t hash(std::string_view data) {
        return zend_hash_func(data.data(), data.size());
    }
}
// 嵌入哈析函数，方便容器使用
namespace std {
    template<> struct hash<php::value> {
        typedef php::value argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept {
            assert(zval_get_type(s) == php::TYPE_STRING);
            return zend_string_hash_func(s);
        }
    };
}

#endif // LIBPHPEXT_HASH_H