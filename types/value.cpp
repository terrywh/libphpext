#include "../phpext.h"

namespace php
{
	value::value(zval* v, bool ref)
	: val_(ref ? v : &value_)
	, ref_(ref)
	{
		if(ref_) {
			ZVAL_UNDEF(&value_);
		}else{
			ZVAL_COPY(val_, v);
		}
	}
	value::value(std::nullptr_t)
	: val_(&value_)
	, ref_(false)
	{
		ZVAL_NULL(val_);
	}
	value::value(const value& w)
	: val_(&value_)
	, ref_(false)
	{
		ZVAL_COPY(val_, w.val_);
	}

	value::value(value&& w)
	: val_(w.ref_ ? w.val_ : &value_)
	, ref_(w.ref_)
	{
		if(w.ref_) {
			ZVAL_UNDEF(&value_);
		}else{
			ZVAL_COPY_VALUE(val_, w.val_);
		}
		w.ref_ = false;
		w.val_ = &w.value_;
		ZVAL_UNDEF(w.val_);
	}

	void value::reset(value&& w) {
		if(val_ != nullptr) {
			_zval_dtor(val_);
		}
		ref_ = w.ref_;
		if(ref_) {
			val_ = w.val_;
		}else{
			val_ = &value_;
			ZVAL_COPY_VALUE(val_, w.val_);
		}
		w.ref_ = false;
		w.val_ = &w.value_;
		ZVAL_UNDEF(w.val_);
	}

	value& value::operator=(const value& w) {
		if(val_ != nullptr) {
			_zval_dtor(val_);
		}
		if(!ref_) {
			val_ = &value_;
			ref_ = false;
		}
		ZVAL_COPY(val_, w.val_);
		return *this;
	}
	value& value::operator=(value&& w)
	{
		if(val_ != nullptr) {
			_zval_dtor(val_);
		}
		if(!ref_) {
			val_ = &value_;
			ref_ = false;
		}
		ZVAL_COPY_VALUE(val_, w.val_);
		w.ref_ = false;
		w.val_ = &w.value_;
		ZVAL_UNDEF(w.val_);
		return *this;
	}

	bool value::operator==(const value& v) const {
		if(val_ == v.val_) return true;
		if(val_ == nullptr || v.val_ == nullptr) return false;
		auto ti = Z_TYPE_INFO_P(val_);
		if(ti != Z_TYPE_INFO_P(v.val_)) return false;
		switch(ti & Z_TYPE_MASK) {
		case IS_NULL:
		case IS_TRUE:
		case IS_FALSE:
			return true;
		case IS_LONG:
			return Z_LVAL_P(val_) == Z_LVAL_P(v.val_);
		case IS_DOUBLE:
			return Z_DVAL_P(val_) == Z_DVAL_P(v.val_);
		case IS_STRING:
			return zend_string_equals(Z_STR_P(val_), Z_STR_P(v.val_));
		case IS_OBJECT:
			return Z_OBJ_P(val_) == Z_OBJ_P(v.val_); // 引用同一个对象
		case IS_REFERENCE:
			return Z_REF_P(val_) == Z_REF_P(v.val_);
		// TODO 暂时不给特殊类型定义相等判定
		// case IS_ARRAY:
		// case IS_RESOURCE:
		default:
			return false;
		}
	}

	std::size_t value::length() {
		if(ref_ && val_ == nullptr) return 0;
		switch(Z_TYPE_P(val_)) {
			case IS_UNDEF:
			case IS_NULL:
				return 0l;
			case IS_FALSE:
			case IS_TRUE:
				return sizeof(zend_bool);
			case IS_LONG:
				return sizeof(zend_long);
			case IS_DOUBLE:
				return sizeof(double);
			case IS_STRING:
				return Z_STRLEN_P(val_);
			case IS_ARRAY:
				return zend_hash_num_elements(Z_ARRVAL_P(val_));
			default: // TODO 暂时不会为其他类型定义 长度 概念
				return 0;
		}
	}

	value::~value() {
		if(!ref_) _zval_dtor(val_);
	}
}
