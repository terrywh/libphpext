#pragma once

namespace php {
	class TYPE {
	public:
		TYPE(zend_uchar t)
		: t_(t) {

		}
		TYPE(const zval* v) {
			t_ = Z_TYPE_P(v);
		}
		std::string name() const {
			switch(t_) {
			case UNDEFINED:
				return "undefined";
			case NULLABLE:
				return "NULL";
			case FALSE:
			case TRUE:
			// case BOOLEAN:
				return "boolean";
			case INTEGER:
				return "integer";
			case FLOAT:
				return "double";
			case STRING:
				return "string";
			case ARRAY:
				return "array";
			case OBJECT:
				return "object";
			case RESOURCE:
				return "resource";
			// case CALLABLE:
			// 	return "callable";
			// case INDIRECT:
			// 	return "indirect";
			// case POINTER:
			// 	return "pointer";	
			default:
				return "unknown type"; // compatible with gettype()
			}
		}
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
		operator zend_uchar() const {
			return t_;
		}
	private:
		zend_uchar t_;
	};
}