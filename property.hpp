#pragma once

namespace php {
	class property {
	private:
		static void set(const value& obj, const string& key, const value& val) {
			zval* object = static_cast<zval*>(obj);
			zend_update_property_ex(Z_OBJCE_P(object), object, key, val);
		}
		static value get(const value& obj, const string& key) {
			zval* object = static_cast<zval*>(obj), rv, *val;
			zend_class_entry* scope = Z_OBJCE_P(object);
			// 参考 zend_read_property 相关代码
			zend_class_entry *old_scope = EG(scope);

			EG(scope) = scope;
			assert(Z_OBJ_HT_P(object)->read_property);
			// !!! 此处获得的 zval 指针可能时临时的对象
			// (例如通过 __get 魔术函数获得的属性)
			val = Z_OBJ_HT_P(object)->read_property(object, key, BP_VAR_IS, nullptr, &rv);
			EG(scope) = old_scope;
			return php::value(val);
		}
		value  ref_; // 需要复制一份对象的引用, 防止对象提前销毁
		string key_;
	public:
		property(const value& ref, const string& key)
		: ref_(ref), key_(key) {
			
		}
		property& operator =(const php::value& val) {
			set(ref_, key_, val);
			return *this;
		}
		operator value() const {
			return get(ref_, key_);
		}
		friend class object;
		friend class class_base;
	};
}
