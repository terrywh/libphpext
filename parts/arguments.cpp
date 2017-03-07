#include "../phpext.h"

namespace php {
	arguments::arguments(std::initializer_list<zend_internal_arg_info> argv)
	:infos_(argv) {
		zend_internal_function_info info1;
		info1.required_num_args = -1;
		info1.class_name = nullptr;
		info1.type_hint = 0;
		info1.return_reference = false;
		info1.allow_null = false;
		info1._is_variadic = false;
		zend_internal_arg_info* info2 = reinterpret_cast<zend_internal_arg_info*>(&info1);
		infos_.insert(infos_.begin(), *info2);
	}
	arguments::arguments(arguments&& info)
	: infos_(std::move(info.infos_)) {

	}

	zend_internal_arg_info of_bool(const char* _name, zend_bool _ref, zend_bool _null) {
		zend_internal_arg_info info;
		info.name = _name;
		info.class_name = nullptr;
		info.type_hint = _IS_BOOL;
		info.pass_by_reference = _ref;
		info.allow_null = _null;
		info.is_variadic = false;
		return info;
	}
	zend_internal_arg_info of_integer(const char* _name, zend_bool _ref, zend_bool _null) {
		zend_internal_arg_info info;
		info.name = _name;
		info.class_name = nullptr;
		info.type_hint = IS_LONG;
		info.pass_by_reference = _ref;
		info.allow_null = _null;
		info.is_variadic = false;
		return info;
	}
	zend_internal_arg_info of_double(const char* _name, zend_bool _ref, zend_bool _null) {
		zend_internal_arg_info info;
		info.name = _name;
		info.class_name = nullptr;
		info.type_hint = IS_DOUBLE;
		info.pass_by_reference = _ref;
		info.allow_null = _null;
		info.is_variadic = false;
		return info;
	}
	zend_internal_arg_info of_string(const char* _name, zend_bool _ref, zend_bool _null) {
		zend_internal_arg_info info;
		info.name = _name;
		info.class_name = nullptr;
		info.type_hint = IS_STRING;
		info.pass_by_reference = _ref;
		info.allow_null = _null;
		info.is_variadic = false;
		return info;
	}
	zend_internal_arg_info of_array(const char* _name, zend_bool _ref, zend_bool _null) {
		zend_internal_arg_info info;
		info.name = _name;
		info.class_name = nullptr;
		info.type_hint = IS_ARRAY;
		info.pass_by_reference = _ref;
		info.allow_null = _null;
		info.is_variadic = false;
		return info;
	}
	zend_internal_arg_info of_callable(const char* _name, zend_bool _null) {
		zend_internal_arg_info info;
		info.name = _name;
		info.class_name = nullptr;
		info.type_hint = IS_CALLABLE;
		info.pass_by_reference = false;
		info.allow_null = _null;
		info.is_variadic = false;
		return info;
	}
	zend_internal_arg_info of_object(const char* _name, const char* _class, zend_bool _null) {
		zend_internal_arg_info info;
		info.name = _name;
		info.class_name = _class;
		info.type_hint = IS_OBJECT;
		info.pass_by_reference = false;
		info.allow_null = _null;
		info.is_variadic = false;
		return info;
	}

	arguments::operator zend_internal_arg_info*() const {
		return const_cast<zend_internal_arg_info*>(infos_.data());
	}

	std::size_t arguments::length() const {
		return infos_.size() - 1;
	}
}
