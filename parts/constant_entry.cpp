#include "../phpext.h"

namespace php {
	constant_entry::constant_entry(const php::string& name, const php::value& v)
	: key_(name), val_(v) {}

	void constant_entry::declare(int module) {
		// 常量声明要求使用 persistent , 这里需要重新创建
		switch(val_.type()) {
		case IS_NULL:
			zend_register_null_constant(key_.c_str(), key_.length(),
				CONST_PERSISTENT, module);
			break;
		case IS_TRUE:
		case IS_FALSE:
			zend_register_bool_constant(key_.c_str(), key_.length(), val_.to_bool(),
				CONST_PERSISTENT, module);
			break;
		case IS_LONG:
			zend_register_long_constant(key_.c_str(), key_.length(),
				val_.to_long(), CONST_PERSISTENT, module);
			break;
		case IS_DOUBLE:
			zend_register_double_constant(key_.c_str(), key_.length(),
				val_.to_double(), CONST_PERSISTENT, module);
			break;
		case IS_STRING:
			zend_register_stringl_constant(key_.c_str(), key_.length(),
				static_cast<php::string&>(val_).data(), static_cast<php::string&>(val_).length(),
				CONST_PERSISTENT, module);
			break;
		default:{
			std::string msg;
			msg.append("constant '");
			msg.append(key_.c_str(), key_.length());
			msg.append("' can not be delared: illegal type");
			throw std::runtime_error(msg);
		}}
	}
	void constant_entry::declare(zend_class_entry* entry) {
		// 常量声明要求使用 persistent , 这里需要重新创建
		switch(val_.type()) {
		case IS_NULL:
			zend_declare_class_constant_null(entry, key_.c_str(), key_.length());
			break;
		case IS_TRUE:
		case IS_FALSE:
			zend_declare_class_constant_bool(entry, key_.c_str(), key_.length(),
				val_.to_bool());
			break;
		case IS_LONG:
			zend_declare_class_constant_long(entry, key_.c_str(), key_.length(),
				val_.to_long());
			break;
		case IS_DOUBLE:
			zend_declare_class_constant_double(entry, key_.c_str(), key_.length(),
				val_.to_double());
			break;
		case IS_STRING:
			zend_declare_class_constant_stringl(entry, key_.c_str(), key_.length(),
				static_cast<php::string&>(val_).c_str(), static_cast<php::string&>(val_).length());
			break;
		default:{
			std::string msg;
			msg.append("constant '");
			msg.append(key_.c_str(), key_.length());
			msg.append("' can not be delared: illegal type");
			throw std::runtime_error(msg);
		}}
	}
}
