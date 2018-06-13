#pragma once

namespace php {
	class TYPE {
	public:
		TYPE(zend_uchar t);
		TYPE(const zval* v);
		std::string name() const;
		enum : zend_uchar {
			UNDEFINED = IS_UNDEF,
			NULLABLE  = IS_NULL,
			BOOLEAN   = _IS_BOOL, // 仅用于参数说明
			TRUE      = IS_TRUE,
			FALSE     = IS_FALSE,
			INTEGER   = IS_LONG,
			FLOAT     = IS_DOUBLE,
			STRING    = IS_STRING,
			ARRAY     = IS_ARRAY,
			OBJECT    = IS_OBJECT,
			RESOURCE  = IS_RESOURCE,
			CALLABLE  = IS_CALLABLE, // 仅用于参数说明
			// 内部类型
			INDIRECT  = IS_INDIRECT,
			POINTER   = IS_PTR,
		};
		operator zend_uchar() const;
	private:
		zend_uchar t_;
	};
}