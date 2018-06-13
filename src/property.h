#pragma once

namespace php {
	class property {
	private:
		static zval* get(const value& obj, const string& key);
		static void set(const value& obj, const string& key, const value& val);
		value  ref_; // 需要复制一份对象的引用, 防止对象提前销毁
		string key_;
	public:
		property(const value& ref, const string& key);
		property& operator =(const php::value& val);
		operator value() const;
		friend class object;
		friend class class_base;
	};
}
