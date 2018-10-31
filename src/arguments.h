#pragma once

#include "type.h"

namespace php {
	class argument {
	public:
		argument(const char* name, bool byref = false, bool nullable = false);
		argument(const char* name, const char* class_name);
		// 注意：基础类型的 type_hint 类型检查实际未检查 (除 Callable 及 类类型 外)
		argument(const char* name, TYPE type_hint, bool byref = false, bool nullable = false);
	private:
		argument(const char* name, const char* class_name, bool byref);
		zend_internal_arg_info arg_;
		friend class arguments;
	};
	class arguments {
	public:
		arguments(std::initializer_list<argument> args);
		arguments(arguments&& a);
		operator zend_internal_arg_info*() const;
		std::uint32_t size() const;
	private:
		std::vector<zend_internal_arg_info> args_;
	};
}