#include "../vendor.h"

#ifndef PHPEXT_TYPES_VALUE_H
#define PHPEXT_TYPES_VALUE_H

namespace phpext
{
namespace types
{
// @ zend_value / zval 结构参数考 zend/zend_types.h:101
class value {
public:
	value(std::nullptr_t)
	:value()
	{
		ZVAL_NULL(val_);
	}
	value(bool v)
	:value()
	{
		if(v) ZVAL_TRUE(val_);
		else ZVAL_FALSE(val_);
	}
	value(int v)
	:value()
	{
		ZVAL_LONG(val_, v);
	}
	value(std::int64_t v)
	:value()
	{
		ZVAL_LONG(val_, v);
	}
	value(double v)
	:value()
	{
		ZVAL_DOUBLE(val_, v);
	}
	value(const char* v)
	:value()
	{
		ZVAL_NEW_STR(val_, zend_string_init(v, std::strlen(v), false));
	}
	operator bool()
	{
		if(Z_TYPE_P(val_) == IS_TRUE) return true;
		return false;
	}
	operator int()
	{
		assert(Z_TYPE_P(val_) == IS_LONG);
		return Z_LVAL_P(val_);
	}
	operator std::int64_t()
	{
		assert(Z_TYPE_P(val_) == IS_LONG);
		return Z_LVAL_P(val_);
	}
	operator double()
	{
		assert(Z_TYPE_P(val_) == IS_DOUBLE);
		return Z_DVAL_P(val_);
	}
	operator const char*()
	{
		assert(Z_TYPE_P(val_) == IS_STRING);
		return Z_STRVAL_P(val_);
	}

	value(const value& w)
	{
		val_ = w.val_;
		addref();
	}
	value(value&& w)
	{
		val_ = w.val_;
		w.val_ = nullptr;
	}
	virtual ~value()
	{
		if(val_ != nullptr)
		{
			zval_dtor(val_);
			efree(val_);
		}
	}

	// assign
	value& operator=(const value& v)
	{
		ZVAL_COPY(val_, v.val_);
		return *this;
	}
	value& operator=(value&& v)
	{
		val_ = v.val_;
		v.val_ = nullptr;
		return *this;
	}

	bool operator==(const value& v)
	{
		return Z_PTR_P(val_) == Z_PTR_P(v.val_);
	}
	bool is_empty()
	{
		switch(Z_TYPE_P(val_)) {
			case IS_UNDEF:
			case IS_NULL:
			case IS_FALSE:
				return true;
			case IS_LONG:
			case IS_DOUBLE:
				return Z_LVAL_P(val_) == 0;
			case IS_STRING:
				return Z_STRLEN_P(val_) == 0;
			case IS_ARRAY:
				return Z_ARRVAL_P(val_)->nNumOfElements == 0;
			default: // TODO how to determine 'empty' for other types?
				return false;
		}
	}
	inline bool is_null() const
	{
		return Z_TYPE_P(val_) == IS_NULL;
	}
	inline bool is_string() const
	{
		return Z_TYPE_P(val_) == IS_STRING;
	}
	inline bool is_long() const
	{
		return Z_TYPE_P(val_) == IS_LONG;
	}
	inline bool is_double() const
	{
		return Z_TYPE_P(val_) == IS_DOUBLE;
	}
	inline bool is_array() const
	{
		return Z_TYPE_P(val_) == IS_ARRAY;
	}
	inline bool is_callable() const
	{
		return zend_is_callable(val_, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr);
	}
	inline void addref()
	{
		Z_TRY_ADDREF_P(val_); // value may hold non-refcounted types
	}
	inline void delref()
	{
		Z_TRY_DELREF_P(val_);
	}
	inline zval* data() const
	{
		return val_;
	}
protected:
	value()
	{
		val_ = (zval*)emalloc(sizeof(zval));
	}
	value(zval* v, bool refer = false)
	{
		val_ = v;
		if(!refer) addref();
	}
	zval* val_;
};

}}

#endif // PHPEXT_TYPES_VALUE_H
