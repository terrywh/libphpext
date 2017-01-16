#include "value.h"

namespace php
{
	value::value()
	:ref_(false)
	{
		val_ = (zval*)emalloc(sizeof(zval));
	}
	value::value(zval* v, bool ref)
	:ref_(ref)
	{
		if(ref_) { // 引用形式需要比较，同时进行引用计数
			val_ = v;
			addref();
		}else{ // 非引用需要自行创建 zval
			val_ = (zval*)emalloc(sizeof(zval));
			ZVAL_COPY(val_, v);
		}
	}
	value::value(std::nullptr_t)
	:value()
	{
		ZVAL_NULL(val_);
	}
	value::value(const value& w)
	:value()
	{
		ZVAL_COPY(val_, w.val_);
	}
	value::value(value&& w)
	{
		val_ = w.val_;
		w.val_ = nullptr;
	}
	value& value::operator=(const value& w)
	{
		_zval_dtor(val_); // 销毁目前持有的对象
		ZVAL_COPY(val_, w.val_);
		return *this;
	}
	value& value::operator=(value&& w)
	{
		// move but assign
		_zval_dtor(val_);
		ZVAL_COPY_VALUE(val_, w.val_);
		ZVAL_UNDEF(w.val_);
		return *this;
	}
	value::~value()
	{
		if(val_ != nullptr)
		{
			_zval_dtor(val_);
			if(!ref_) efree(val_);
		}
	}
}
