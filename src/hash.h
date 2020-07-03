#ifndef LIBPHPEXT_HASH_H
#define LIBPHPEXT_HASH_H

#include "vendor.h"

namespace php {
    //
    std::uint32_t crc32(std::string_view data);
    // PHP 内置哈系表对应哈系函数
    inline std::uint64_t hash(std::string_view data) {
        return zend_hash_func(data.data(), data.size());
    }
}

#endif // LIBPHPEXT_HASH_H