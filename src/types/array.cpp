#include "array.h"

namespace core
{
namespace types
{

	array::array()
	{
		_array = (zend_array*)emalloc(sizeof(zend_array));
		_zend_hash_init(_array, 0, ZVAL_PTR_DTOR, false);
	}

	array::array(int size):array()
	{
		_zend_hash_init(_array, size, ZVAL_PTR_DTOR, false);
	}

	array::array(zend_array* ht, bool copy)
	{
		_array = ht;
		if(copy) ++GC_REFCOUNT(ht);
	}

	array::array(const array& v)
	{
		++GC_REFCOUNT(v._array);
		_array = v._array;
	}

	array::array(array&& v)
	{
		_array = v._array;
		v._array = nullptr;
	}

	array::~array()
	{
		if(_array != nullptr && GC_REFCOUNT(_array) == 1)
		{
			zend_hash_destroy(_array);
		}
	}

	array_value_key array::operator[](char* key)
	{
		return array_value_key(_array, key);
	}

	array_value_idx array::operator[](int index)
	{
		return array_value_idx(_array, index);
	}

	std::uint32_t array::length() const {
		return zend_hash_num_elements(_array);
	}

}}
