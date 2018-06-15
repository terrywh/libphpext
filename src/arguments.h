#pragma once

namespace php {
	class argument {
	public:
		argument(const char* name, bool byref = false, bool nullable = false);
		argument(const char* name, const char* class_name, bool nullable = false);
		// 注意：基础类型的 type_hint 检查 PHP 暂时还未实现
		argument(const char* name, TYPE type_hint, bool byref = false, bool nullable = false);
	private:
		argument(const char* name, const char* class_name, zend_uchar type_hint, bool byref, bool nullable);
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