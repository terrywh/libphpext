#pragma once

namespace php {
	class arguments {
	public:
		arguments();
		arguments(arguments&& info);

		arguments& _bool(const char* _name, zend_bool _ref = false, zend_bool _null = false);
		arguments& _long(const char* _name, zend_bool _ref = false, zend_bool _null = false);
		arguments& _double(const char* _name, zend_bool _ref = false, zend_bool _null = false);
		arguments& _string(const char* _name, zend_bool _ref = false, zend_bool _null = false);
		arguments& _array(const char* _name, zend_bool _ref = false, zend_bool _null = false);
		arguments& _callable(const char* _name, zend_bool _null = false);
		arguments& _object(const char* _name, const char* _class = nullptr, zend_bool _null = false);

		arguments&& done();

		std::size_t length() const;
		explicit operator zend_internal_arg_info*() const;
	private:
		std::vector<zend_internal_arg_info> infos_;
	};
}
