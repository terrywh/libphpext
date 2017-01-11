#include <zend.h>
#include "array_value.h"

#ifndef CORE_TYPES_ARRAY_H
#define CORE_TYPES_ARRAY_H

namespace core
{
namespace types
{
	
	class array
	{
	public:
		array();
		array(int size);
		array(zend_array* ht, bool copy = true);
		array(const array& ht);
		array(array&& ht);
		virtual ~array();
		// item access
		array_value_key operator[](char* key);
		array_value_idx operator[](int index);
		std::uint32_t length() const;
		// refcount
		inline void addref()
		{
			++GC_REFCOUNT(_array);
		}
		// refcount
		inline void delref()
		{
			--GC_REFCOUNT(_array);
		}

		inline zend_array* data() const
		{
			return _array;
		}
	protected:
		zend_array* _array;
	};
}}

#endif // CORE_TYPES_ARRAY_H
