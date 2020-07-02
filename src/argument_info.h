#ifndef LIBPHPEXT_ARGUMENT_INFO_H
#define LIBPHPEXT_ARGUMENT_INFO_H

#include "vendor.h"
#include "value.h"

#undef ZEND_TYPE_INIT_MASK
#define ZEND_TYPE_INIT_MASK(_type_mask) { NULL, static_cast<uint32_t>(_type_mask) }

namespace php {
	// TODO 实现对 zend_type_list 的封装，以提供
    // 单参数描述信息
	class argument_info: public zend_internal_arg_info {
	public:
		// 无类型参数
		argument_info(const char* name, bool byref = false, bool nullable = false, bool variadic = false)
		: zend_internal_arg_info { name, ZEND_TYPE_INIT_CODE(TYPE_MIXED, nullable, _ZEND_ARG_INFO_FLAGS(byref, variadic)), nullptr } {}
		// 基础类型参数
		argument_info(const char*  name, type_code_t type_hint, bool byref = false, bool nullable = false, bool variadic = false)
		: zend_internal_arg_info { name, ZEND_TYPE_INIT_CODE(type_hint, nullable, _ZEND_ARG_INFO_FLAGS(byref, variadic)), nullptr } {}

		// 类类型参数
		argument_info(const char* name, zend_class_entry* ce, bool nullable = false)
		: zend_internal_arg_info { name, ZEND_TYPE_INIT_CE(ce, nullable, false), nullptr }  {}
		// 类类型参数
		argument_info(const char* name, const char* class_name, bool nullable = false)
		: zend_internal_arg_info { name, ZEND_TYPE_INIT_CLASS_CONST(class_name, nullable, false), nullptr } {}
	};

	// 返回信息
	class return_info: public zend_internal_arg_info {
	public:
		// 无返回类型描述
		return_info()
		: zend_internal_arg_info { (const char*)(zend_uintptr_t)-1, ZEND_TYPE_INIT_NONE(0), nullptr } {}
		// 返回基础类型
		return_info(type_code_t type_hint, bool byref = false, bool nullable = false)
		: zend_internal_arg_info { (const char*)(zend_uintptr_t)-1, ZEND_TYPE_INIT_CODE(type_hint, nullable, _ZEND_ARG_INFO_FLAGS(byref, 0)), nullptr } {}
		// 返回类类型
		return_info(zend_string* class_name, bool nullable = false)
		: zend_internal_arg_info { (const char*)(zend_uintptr_t)-1, ZEND_TYPE_INIT_CLASS_CONST(class_name, nullable, 0), nullptr } {}
		// 返回类类型
		return_info(zend_class_entry* ce, bool nullable = false)
		: zend_internal_arg_info { (const char*)(zend_uintptr_t)-1, ZEND_TYPE_INIT_CE(ce, nullable, 0), nullptr } {}
		
		void required(int required) {
			name = (const char*)(zend_uintptr_t)(required);
		}
		int required() {
			return (zend_uintptr_t)name;
		}
	};
}

#endif // LIBPHPEXT_ARGUMENT_INFO_H
