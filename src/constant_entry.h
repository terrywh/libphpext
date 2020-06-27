#ifndef LIBPHPEXT_CONSTANT_ENTRY_H
#define LIBPHPEXT_CONSTANT_ENTRY_H

#include "forward.h"
#include "value.h"

namespace php {
	// 常量
	class constant_entry: public zend_constant {
	public:
		// 常量定义，可选文档注释
		constant_entry(std::string_view k, const php::value& v)
		: constant_entry(k, v, {}) {
			
		}
		// 常量定义，可选文档注释
		constant_entry(std::string_view key, const php::value& val, std::string_view doc);
		// void declare(zend_class_entry* ce) {
		// 	zend_register_constant(this);
		// 	zend_declare_class_constant_ex(ce, name, &value, ZEND_ACC_PUBLIC, comment);
		// }
		// 添加成员
		zend_string* comment;
	};
}

#endif // LIBPHPEXT_CONSTANT_ENTRY_H
