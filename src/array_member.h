#pragma once

namespace php {
	class array_member: public value_fn {
	private:
		value&       arr_;
		zend_ulong   idx_;
		HashPosition pos_;
		string       key_;
	public:
		array_member(value& arr, const string& key);
		array_member(value& arr, zend_ulong idx);
		array_member(value& arr, HashPosition pos);
		array_member& operator =(const value& val);
		bool exists() const;
		// ---------------------------------------------------------
		operator value() const;
		virtual value ptr() const override;
		zval* raw() const; // 不适用 operator zval* 会与 value 构造发生混淆


	};
}
