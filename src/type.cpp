#include "type.h"

namespace php {
    //
    type_item::operator zend_type() const {
        if(attr_ & IS_CLASS_NAME) 
            return zend_type ZEND_TYPE_INIT_CLASS_CONST(name_, allow_null(), 0u);
        else
            return zend_type ZEND_TYPE_INIT_CODE(code_, allow_null(), _ZEND_ARG_INFO_FLAGS(pass_byref(), is_variadic()));
    }
    //
    type_item type_item::operator |(const type_attr& attr) const {
        type_item ti(*this);
        ti.attr_ |= static_cast<std::uint32_t>(attr);
        return ti;
    }
    type_item operator|(const type_code& code, const type_attr& attr) {
        return type_item(code) | attr;
    }
    // 实际输出的类型
    type_list::operator zend_type() const {
        zend_type type;
        zend_type_list* list = static_cast<zend_type_list*>(pemalloc(ZEND_TYPE_LIST_SIZE(types_.size()), true));
        list->num_types = types_.size();
        std::memcpy(&list->types, types_.data(), sizeof(zend_type) * types_.size());
        ZEND_TYPE_SET_LIST(type, list);
        return type;
    }
    // 构建 union type
    type_list operator |(const type_code& code1, const type_code& code2) {
        return type_item{code1} | code2;
    }
    // 构建 union type
    type_list operator |(const type_desc& code1, const type_code& code2) {
        type_list tl;
        tl.types_.push_back(code1);
        return tl | code2;
    }
    // 加入 union type
    type_list operator |(const type_list& t1,  const type_code& code2) {
        type_list t2 {t1};
        t2.types_.push_back( type_item{code2} );
        return t2;
    }
}
