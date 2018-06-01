#include "../phpext.h"

namespace php {
	array_item_assoc::array_item_assoc(const php::array& arr, const php::string& key)
	: array_(arr)
	, assoc_(key) {
		zval* vv = zend_hash_find_ind(array_, assoc_);
		if(vv == nullptr) ZVAL_UNDEF(&value_);
		else ZVAL_COPY(&value_, vv);
	}
	array_item_assoc& array_item_assoc::operator =(const php::value& val) {
		value::operator =(val);
		zend_hash_update(array_, assoc_, &value_);
		addref();
		return *this;
	}
	array_item_assoc& array_item_assoc::operator =(php::value&& val) {
		value::operator =(std::move(val));
		zend_hash_update(array_, assoc_, &value_);
		addref();
		return *this;
	}
	array_item_assoc::operator php::value() {
		return php::value(value_);
	}
	array_item_index::array_item_index(const php::array& arr, zend_ulong key)
	: array_(arr)
	, index_(key) {
		zval* vv = zend_hash_index_find(array_, index_);
		if(vv == nullptr) {
			ZVAL_UNDEF(&value_);
		}else{
			ZVAL_COPY(&value_, vv);
		}
	}
	array_item_index& array_item_index::operator =(const php::value& val) {
		value::operator =(val);
		zend_hash_index_update(array_, index_, &value_);
		addref();
		return *this;
	}
	array_item_index& array_item_index::operator =(php::value&& val) {
		value::operator =(val);
		zend_hash_index_update(array_, index_, &value_);
		addref();
		return *this;
	}
	array_item_index::operator php::value() {
		return php::value(value_);
	}
}
