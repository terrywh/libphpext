#pragma once

namespace php {
	class TYPE {
	public:
		TYPE(zend_uchar t);
		TYPE(const zval* v);
		std::string name() const;
		static const TYPE UNDEFINED;
		static const TYPE NULLABLE;
		static const TYPE BOOLEAN; // 仅用于参数说明
		static const TYPE TRUE;
		static const TYPE FALSE;
		static const TYPE INTEGER;
		static const TYPE FLOAT;
		static const TYPE STRING;
		static const TYPE ARRAY;
		static const TYPE OBJECT;
		static const TYPE RESOURCE;
		static const TYPE CALLABLE; // 仅用于参数说明
		static const TYPE INDIRECT; // 内部类型
		static const TYPE POINTER;  // 内部类型
		operator zend_uchar() const;
	private:
		zend_uchar t_;
	};
}