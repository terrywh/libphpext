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

		// item access
		array_value operator[](const char* key)
		{
			zval* v = zend_hash_str_find(Z_ARRVAL_P(val_), key, std::strlen(key));
			if(v == nullptr) return value(nullptr);
			else {
				Z_TRY_ADDREF_P(v);
				return value(v, /*refer=*/true);
			}
		}
		array_value operator[](int index)
		{
			zval* v = zend_hash_index_find(Z_ARRVAL_P, index);
			if(v == nullptr) return value(nullptr);
			else {
				Z_TRY_ADDREF_P(v);
				return value(v, /*refer=*/true);
			}
		}

	};
}}

#endif // PHPEXT_TYPES_ARRAY_H
