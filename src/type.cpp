#include "vendor.h"
#include "type.h"

namespace php {
	TYPE::TYPE(zend_uchar t)
	: t_(t) {

	}
	TYPE::TYPE(const zval* v) {
		t_ = Z_TYPE_P(v);
	}
	std::string TYPE::name() const {
		switch(t_) {
		case IS_UNDEF:
			return "undefined";
		case IS_NULL:
			return "NULL";
		case IS_FALSE:
		case IS_TRUE:
		case _IS_BOOL:
			return "boolean";
		case IS_LONG:
			return "integer";
		case IS_DOUBLE:
			return "double";
		case IS_STRING:
			return "string";
		case IS_ARRAY:
			return "array";
		case IS_OBJECT:
			return "object";
		case IS_RESOURCE:
			return "resource";
		case IS_CALLABLE:
			return "callable";
		case IS_INDIRECT:
			return "indirect";
		case IS_PTR:
			return "pointer";	
		default:
			return "unknown type"; // compatible with gettype()
		}
	}
	TYPE::operator zend_uchar() const {
		return t_;
	}
	const TYPE TYPE::UNDEFINED = zend_uchar(IS_UNDEF);
	const TYPE TYPE::NULLABLE  = zend_uchar(IS_NULL);
	const TYPE TYPE::BOOLEAN   = zend_uchar(_IS_BOOL);
	const TYPE TYPE::TRUE      = zend_uchar(IS_TRUE);
	const TYPE TYPE::FALSE     = zend_uchar(IS_FALSE);
	const TYPE TYPE::INTEGER   = zend_uchar(IS_LONG);
	const TYPE TYPE::FLOAT     = zend_uchar(IS_DOUBLE);
	const TYPE TYPE::STRING    = zend_uchar(IS_STRING);
	const TYPE TYPE::ARRAY     = zend_uchar(IS_ARRAY);
	const TYPE TYPE::OBJECT    = zend_uchar(IS_OBJECT);
	const TYPE TYPE::RESOURCE  = zend_uchar(IS_RESOURCE);
	const TYPE TYPE::CALLABLE  = zend_uchar(IS_CALLABLE);
	const TYPE TYPE::INDIRECT  = zend_uchar(IS_INDIRECT);
	const TYPE TYPE::POINTER   = zend_uchar(IS_PTR);
}