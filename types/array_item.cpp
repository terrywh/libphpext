#include "../phpext.h"

namespace php {
	array_item::array_item(php::array& arr, const php::value& key)
	: array_(arr)
	, key_(key) {
		zval* vv;
		if(key_.is_long()) {
			vv = zend_hash_index_find(array_, key_);
		}else{
			vv = zend_hash_find(array_, key_);
		}
		if(vv != nullptr) {
			ZVAL_COPY(&value_, vv);
		}
	}
	array_item& array_item::operator =(const php::value& val) {
		if(is_undefined()) {
			if(key_.is_long()) {
				zend_hash_index_add(array_, key_, val);
			}else{
				zend_hash_add(array_, key_, val);
			}
		}else if(key_.is_long()){
			zend_hash_index_update(array_, key_, val);
		}else{
			zend_hash_update(array_, key_, val);
		}
		value::operator=(val);
		addref();
		return *this;
	}
	array_item& array_item::operator =(php::value&& val) {
		if(is_undefined()) {
			if(key_.is_long()) {
				zend_hash_index_add(array_, key_, val);
			}else{
				zend_hash_add(array_, key_, val);
			}
		}else if(key_.is_long()){
			zend_hash_index_update(array_, key_, val);
		}else{
			zend_hash_update(array_, key_, val);
		}
		value::operator=(std::move(val));
		addref();
		return *this;
	}
}
