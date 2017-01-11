#include "value.h"

namespace core
{
namespace types
{
	boost::pool<user_allocator_zend> value::pool(sizeof(zval));

	value::value()
	{
		val_ = (zval*)pool.malloc();
		ZVAL_UNDEF(val_);
	}
	value::value(std::nullptr_t v)
	{
		val_ = (zval*)pool.malloc();
		ZVAL_NULL(val_);
	}
	value::value(const value& v)
	{
		val_ = (zval*)pool.malloc();
		ZVAL_COPY(val_, v.val_); // GC_REFCOUNT ++
	}
	// 移动构造
	value::value(value&& v)
	{
		val_ = v.val_;
		v.val_ = nullptr;
	}
	value::value(zval* v, bool copy)
	{
		if(copy)
		{
			val_ = (zval*)pool.malloc();
			ZVAL_COPY(val_, v);
		}
		else
		{
			val_ = v;
		}
	}
	// beware: destroctor will destroy zval's content
	value::~value()
	{
		if(val_ != nullptr)
		{
			zval_dtor(val_);
			if(pool.is_from(val_))
			{
			 	pool.free(val_);
			}
		}
	}

	value::value(bool v)
	{
		val_ = (zval*)pool.malloc();
		if(v) ZVAL_TRUE(val_);
		else ZVAL_FALSE(val_);
	}
	value::value(int v)
	{
		val_ = (zval*)pool.malloc();
		ZVAL_LONG(val_, v)
	}
	value::value(std::int64_t v)
	{
		val_ = (zval*)pool.malloc();
		ZVAL_LONG(val_, v)
	}
	value::value(double v)
	{
		val_ = (zval*)pool.malloc();
		ZVAL_DOUBLE(val_, v)
	}
	value::value(const char* v)
	{
		val_ = (zval*)pool.malloc();
		ZVAL_NEW_STR(val_, zend_string_init(v, std::strlen(v), 0));
	}
	// @ zend_value / zval 结构参数考 zend/zend_types.h:101
	// 从 value 获取 c++ 数据
	// 注意：无类型转换、检查（可能读取非法数据）
	// ----------------------------------------------------
	value::operator bool()
	{
		if(Z_TYPE_P(val_) == IS_TRUE) return true;
		return false;
	}
	value::operator std::int64_t()
	{
		return Z_LVAL_P(val_);
	}
	value::operator double()
	{
		return Z_DVAL_P(val_);
	}
	value::operator char*()
	{
		return Z_STRVAL_P(val_);
	}

	// 赋值
	value& value::operator=(const value& v)
	{
		ZVAL_COPY(val_, v.val_);
		return *this;
	}
	value& value::operator=(value&& v)
	{
		val_ = v.val_;
		v.val_ = nullptr;
		return *this;
	}

	bool value::is_empty()
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

	bool value::operator==(const value& v)
	{
		return Z_PTR_P(val_) == Z_PTR_P(v.val_);
	}

}}
