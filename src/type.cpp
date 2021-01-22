#include "type.h"

namespace php {
    //
    type_desc::type_desc(type_code type_hint, bool nullable, bool byref, bool variadic)
    : type_ ZEND_TYPE_INIT_CODE(static_cast<std::uint32_t>(type_hint), 
        static_cast<std::uint32_t>(nullable),
        _ZEND_ARG_INFO_FLAGS(static_cast<std::uint32_t>(byref), static_cast<std::uint32_t>(variadic))
    ) {}
    //
    type_desc::type_desc(zend_class_entry* ce, bool nullable) 
    : type_ ZEND_TYPE_INIT_CE(ce, static_cast<std::uint32_t>(nullable), 0u) {}
    //
    type_desc::type_desc(const char* class_name, bool nullable)
    : type_ ZEND_TYPE_INIT_CLASS_CONST(class_name, static_cast<std::uint32_t>(nullable), 0u) {}
    //
    type_list type_desc::operator |(const type_desc& desc) const {
        type_list empty;
        type_list list1 = empty | *this;
        return list1 | desc;
    }
    // 尽量不改变原有用法语义
    type_list type_list::operator |(const type_desc& desc) const {
        type_list list;
        list.types_.assign(types_.begin(), types_.end());
        list.types_.push_back(desc);
        return list;
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
}