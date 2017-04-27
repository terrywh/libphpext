#include "../phpext.h"

namespace php {
	array array::clone(const array& a1) {
		array a2(a1.length());
		zend_hash_copy(a2.arr_, a1.arr_, nullptr);
	}
	array::~array() {
		if(arr_ != nullptr && --GC_REFCOUNT(arr_) == 0) {
			zend_array_destroy(arr_);
		}
	}
	array::array(const array& arr):arr_(arr.arr_) {
		++GC_REFCOUNT(arr_);
	}
	array::array(array& arr):arr_(arr.arr_) {
		arr.arr_ = nullptr;
	}
	array::array(zend_array* arr):arr_(arr) {
		++GC_REFCOUNT(arr_);
	}
	array::array(std::size_t size) {
		arr_ = (zend_array*)emalloc(sizeof(zend_array));
		_zend_hash_init(arr_, size, ZVAL_PTR_DTOR, 0 ZEND_FILE_LINE_RELAY_CC);
	}
	value& array::at(std::size_t idx) {
		value* item = (value*)zend_hash_index_find(arr_, idx);
		if(item == nullptr) {
			item = (value*)zend_hash_index_add_empty_element(arr_, idx);
		}
		return *item;
	}
	value& array::at(const char* key, std::size_t len) {
		zend_string* key_ = zend_string_init(key, len, false);
		value* item = (value*)zend_hash_find(arr_, key_);
		if(item == nullptr) {
			item = (value*)zend_hash_add_empty_element(arr_, key_);
		}
		zend_string_release(key_);
		return *item;
	}

}
