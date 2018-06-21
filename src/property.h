#pragma once

namespace php {
	class property: public value_fn {
	private:
		static zval* get(zval* obj, const string& key, zval* rv);
		static void set(zval* obj, const string& key, const value& val);
		value  ref_; // 需要复制一份对象的引用, 防止对象提前销毁
		string key_;
		zval   val_; // 虚拟属性缓存空间
	public:
		property(const value& ref, const string& key);
		property& operator =(const value& val);
		// !!! 虚拟属性不适用
		virtual value ptr() const override;
		virtual zval* raw() const override; // 防止与 value 构造冲突不适用 operator 形式

		friend class object;
		friend class class_base;
	};
}
