#include "string.h"
#include "value.h"
#include "string_builder.h"

namespace php {
    string_builder string::build() {
        return string_builder();
    }
    // 创建指定长度的字符串
    value string::create(std::size_t size, bool persist) {
        // 新创建（不用增加引用计数）
        return value(zend_string_alloc(size, persist), false); 
    }
    // 创建字符串（复制）
    value string::create(std::string_view s, bool persist) {
        // 新创建（不用增加引用计数）
        return value(zend_string_init(s.data(), s.size(), persist), false);
    }
    
}
