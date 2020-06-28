#include "value.h"

namespace php {
    // 特化 callable 实现
    template <>
    template <>
    callable* value_t<true>::as() const noexcept {
        assert(zend_is_callable(&value_, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr));
        return reinterpret_cast<callable*>(&value_);
    }
    // PHP 数据序列化
    std::ostream& operator << (std::ostream& os, const value& data) {
        if(!data.is(TYPE_STRING)) {
            smart_str buffer {nullptr, false};
            php_json_encode(&buffer, data, PHP_JSON_UNESCAPED_UNICODE);
            os.write(buffer.s->val, buffer.s->len);
            smart_str_free_ex(&buffer, false);
        }
        else {
            std::string_view sv = data;
            os.write(sv.data(), sv.size());
        }
		return os;
	}
}