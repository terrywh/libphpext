#include "vendor.h"
#include "constant_entry.h"

namespace php {
	constant_entry::constant_entry(const php::string& k, const php::value& v)
	: key_(zend_string_init(k.c_str(), k.size(), 1)) {
		switch(Z_TYPE_P(static_cast<zval*>(v))) {
		case IS_NULL:
		case IS_TRUE:
		case IS_FALSE:
		case IS_LONG:
		case IS_DOUBLE:
			ZVAL_COPY_VALUE(static_cast<zval*>(val_), static_cast<zval*>(v));
			break;
		case IS_STRING:
			ZVAL_STR(static_cast<zval*>(val_), zend_string_dup(static_cast<zend_string*>(v), 1));
			break;
		default:
			assert(0 && "常量类型受限");
		}
	}
	//
	void constant_entry::declare(int module) {
		zend_constant c;
		ZVAL_COPY(&c.value, val_);
		c.flags = CONST_PERSISTENT;
		c.name = key_;
		c.module_number = module;
		int r = zend_register_constant(&c);
		assert(r == SUCCESS && "声明常量失败");
	}
	//
	void constant_entry::declare(zend_class_entry* ce) {
		int r = zend_declare_class_constant_ex(ce, key_, val_, ZEND_ACC_PUBLIC, nullptr);
		assert(r == 0 && "声明常量失败");
	}
}
