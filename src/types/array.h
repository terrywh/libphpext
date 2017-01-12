#include "../vendor.h"
#include "value.h"
#include "array_value.h"

#ifndef PHPEXT_TYPES_ARRAY_H
#define PHPEXT_TYPES_ARRAY_H

namespace phpext
{
namespace types
{

	class array: public value
	{
	public:
		array(const value& a)
		:value(a)
		{
			assert(a.is_array());
		}
		array(value&& a)
		:value(std::move(a))
		{
			assert(a.is_array());
		}
		array()
		:value()
		{
			Z_ARRVAL_P(val_) = (zend_array*)emalloc(sizeof(zend_array));
			_zend_hash_init(Z_ARRVAL_P(val_), 0, ZVAL_PTR_DTOR, false);
		}
		array(int size)
		:value()
		{
			Z_ARRVAL_P(val_) = (zend_array*)emalloc(sizeof(zend_array));
			_zend_hash_init(Z_ARRVAL_P(val_), size, ZVAL_PTR_DTOR, false);
		}
		// item access
		array_value_key operator[](char* key)
		{
			return array_value_key(Z_ARRVAL_P(val_), key);
		}
		array_value_idx operator[](int index)
		{
			return array_value_idx(Z_ARRVAL_P(val_), index);
		}
		std::uint32_t length() const
		{
			return zend_hash_num_elements(Z_ARRVAL_P(val_));
		}
	};
}}

#endif // PHPEXT_TYPES_ARRAY_H
