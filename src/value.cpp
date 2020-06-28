#include "value.h"

namespace php {
    // 特化 callable 实现
    template <>
    template <>
    callable* value_t<true>::as() const noexcept {
        assert(zend_is_callable(&value_, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr));
        return reinterpret_cast<callable*>(&value_);
    }
}