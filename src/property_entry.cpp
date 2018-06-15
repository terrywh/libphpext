#include "phpext.h"

namespace php {
	property_entry::property_entry(const php::string& name, const php::value& v, int access)
	: key_(zend_string_init(name.c_str(), name.size(), 1))
	, acc_(access) {
		switch(Z_TYPE_P(static_cast<zval*>(v))) {
		case IS_NULL:
		case IS_TRUE:
		case IS_FALSE:
		case IS_LONG:
		case IS_DOUBLE:
			ZVAL_COPY_VALUE(val_, v);
			break;
		case IS_STRING: {
			zend_string* s = v;
			ZVAL_NEW_STR(val_, zend_string_init(s->val, s->len, 1));
			break;
		}
		default:
			assert(0 && "属性类型受限");
		}
	}
	property_entry::property_entry(property_entry&& entry)
	: key_(std::move(entry.key_))
	, val_(std::move(entry.val_))
	, acc_(entry.acc_) {
		// entry.acc_ = 0;
	}
	void property_entry::declare(zend_class_entry* entry) {
		zend_declare_property_ex(entry, key_, val_, acc_, nullptr);
		ZVAL_UNDEF(key_.raw());
		ZVAL_UNDEF(val_.raw());
	}
}
