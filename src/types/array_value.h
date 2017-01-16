#include "../vendor.h"
#include "value.h"

#ifndef PHPEXT_TYPES_ARRAY_VALUE_H
#define PHPEXT_TYPES_ARRAY_VALUE_H

namespace phpext
{
namespace types
{
	class array_value_key: public value
	{
	public:
		array_value_key(zval* array, const char* k)
		:arr_(a)
		{
			key_ = zend_string_init(k, std::strlen(k), false);
			val_ = zend_hash_find(Z_ARRVAL_P(arr_), key_);
			if(val_ == nullptr)
			{
				zval undef;
				ZVAL_UNDEF(&undef);
				val_ = _zend_hash_add(Z_ARRVAL_P(arr_), key_, &undef);
			}else{
				addref();
			}
		}
		virtual ~array_value_key()
		{
			if(Z_TYPE_P(val_) == IS_UNDEF)
			{ // cleanup empty placeholder
				zend_hash_del(Z_ARRVAL_P(arr_), key_);
				// delref();
			}else{
				delref();
			}
			zend_string_release(key_);
			val_ = nullptr;
			// ~value
		}
	protected:
		zval*        arr_;
		zend_string* key_;
	};

	class array_value_idx: public value
	{
	public:
		array_value_idx(zval* a, zend_ulong i)
		:arr_(a), idx_(i)
		{
			val_ = zend_hash_index_find(Z_ARRVAL_P(arr_), idx_);
			if(val_ == nullptr)
			{ // placeholder
				zval undef;
				ZVAL_UNDEF(&undef);
				val_ = _zend_hash_index_add(Z_ARRVAL_P(arr_), idx_, &undef);
			}else{
				addref();
			}
		}
		virtual ~array_value_idx()
		{
			if(Z_TYPE_P(val_) == IS_UNDEF)
			{ // cleanup empty placeholder
				zend_hash_index_del(Z_ARRVAL_P(arr_), idx_);
			}else{
				delref();
			}
			val_ = nullptr;
			// ~value
		}
	protected:
		zval*       arr_;
		zend_ulong  idx_;
	};

}}

#endif // PHPEXT_TYPES_ARRAY_VALUE_H
