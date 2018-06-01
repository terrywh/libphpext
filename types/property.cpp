#include "../phpext.h"

namespace php {
	property::property(const php::object& obj, const php::string& name)
	: object_(obj), name_(name) {
		// 下面流程来自 Zend_API.c zend_read_property 函数
		zval rv, *value;
		zend_class_entry *old_scope = EG(scope);
		EG(scope) = static_cast<zend_class_entry*>(object_);
		if (!Z_OBJ_HT_P(static_cast<zval*>(object_))->read_property) {
			EG(scope) = old_scope;
			throw php::exception("cannot access object property without read_property handler");
		}
		value = Z_OBJ_HT_P(static_cast<zval*>(object_))->read_property(object_, name, /* silent */ BP_VAR_IS, NULL, &rv);
		EG(scope) = old_scope;
		ZVAL_COPY(&value_, value);
	}
	property& property::operator =(const php::value& val) {
		value::operator=(val);
		zend_update_property_ex(object_, object_, name_, &value_);
	}
	property& property::operator =(php::value&& val) {
		value::operator=(std::move(val));
		zend_update_property_ex(object_, object_, name_, &value_);
	}
}
