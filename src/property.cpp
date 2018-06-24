#include "vendor.h"
#include "property.h"

namespace php {
	zval* property::get(zval* obj, const string& key, zval* rv) {
		zval *val;
		zend_class_entry* scope = Z_OBJCE_P(obj);
		// 参考 zend_read_property 相关代码
		zend_class_entry *old_scope = EG(scope);

		EG(scope) = scope;
		assert(Z_OBJ_HT_P(obj)->read_property);
		// !!! 此处获得的 zval 指针可能时临时的对象
		// (例如通过 __get 魔术函数获得的属性)
		val = Z_OBJ_HT_P(obj)->read_property(obj, key, BP_VAR_IS, nullptr, rv);
		EG(scope) = old_scope;
		return val;
	}
	void property::set(zval* obj, const string& key, const value& val) {
		zend_update_property_ex(Z_OBJCE_P(obj), obj, key, val);
	}
	
	property::property(const value& ref, const string& key)
	: ref_(ref), key_(key) {
		
	}
	property& property::operator =(const value& val) {
		set(ref_, key_, val);
		return *this;
	}
	value property::ptr() const {
		return value(raw(), true);
	}
	zval* property::raw() const {
		return get(ref_, key_, const_cast<zval*>(&val_));
	}
}
