#include <cstring>

#include "value.h"

#ifndef CORE_TYPES_ARRAY_VALUE_H
#define CORE_TYPES_ARRAY_VALUE_H

namespace core
{
namespace types
{
	class array_value_key: public value
	{
	public:
		array_value_key(zend_array* a, const char* k)
		:arr_(a)
		{
			key_ = zend_string_init(k, std::strlen(k), false);
			val_ = zend_hash_find(arr_, key_);
			if(val_ == nullptr)
			{
				zval undef;
				ZVAL_UNDEF(&undef);
				val_ = _zend_hash_add(arr_, key_, &undef);
			}
		}
		virtual ~array_value_key()
		{
			if(Z_TYPE_P(val_) == IS_UNDEF)
			{ // cleanup empty placeholder
				zend_hash_del(arr_, key_);
			}
			zend_string_release(key_);
		}
	protected:
		zend_array*  arr_;
		zend_string* key_;
	};

	class array_value_idx: public value
	{
	public:
		array_value_idx(zend_array* a, zend_ulong i)
		:arr_(a), idx_(i)
		{
			val_ = zend_hash_index_find(arr_, idx_);
			if(val_ == nullptr)
			{ // placeholder
				zval undef;
				ZVAL_UNDEF(&undef);
				val_ = _zend_hash_index_add(arr_, idx_, &undef);
			}
		}
		virtual ~array_value_idx()
		{
			if(Z_TYPE_P(val_) == IS_UNDEF)
			{ // cleanup empty placeholder
				zend_hash_index_del(arr_, idx_);
			}
		}
	protected:
		zend_array* arr_;
		zend_ulong  idx_;
	};

}}

#endif // CORE_TYPES_ARRAY_VALUE_H
