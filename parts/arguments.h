#pragma once

namespace php {
	zend_internal_arg_info of_mixed(const char* _name, zend_bool _ref = false, zend_bool _null = false);
	zend_internal_arg_info of_bool(const char* _name, zend_bool _ref = false, zend_bool _null = false);
	zend_internal_arg_info of_integer(const char* _name, zend_bool _ref = false, zend_bool _null = false);
	zend_internal_arg_info of_float(const char* _name, zend_bool _ref = false, zend_bool _null = false);
	zend_internal_arg_info of_string(const char* _name, zend_bool _ref = false, zend_bool _null = false);
	zend_internal_arg_info of_array(const char* _name, zend_bool _ref = false, zend_bool _null = false);
	zend_internal_arg_info of_callable(const char* _name, zend_bool _null = false);
	zend_internal_arg_info of_object(const char* _name, const char* _class = nullptr, zend_bool _null = false);

	class arguments {
	public:
		arguments(std::initializer_list<zend_internal_arg_info> argv);
		arguments(arguments&& info);
		std::size_t length() const;
		explicit operator zend_internal_arg_info*() const;
	private:
		std::vector<zend_internal_arg_info> infos_;
	};
}
