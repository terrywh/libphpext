#include "phpext.h"

namespace php {
	TYPE::TYPE(zend_uchar t)
	: t_(t) {

	}
	TYPE::TYPE(const zval* v) {
		t_ = Z_TYPE_P(v);
	}
	std::string TYPE::name() const {
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
	TYPE::operator zend_uchar() const {
		return t_;
	}
}