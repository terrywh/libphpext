#include "phpext.h"

namespace php {
	array_member::array_member(value& arr, const string& key)
	: arr_(arr)
	, idx_(-1)
	, pos_(HT_INVALID_IDX)
	, key_(key) {
		
	}
	array_member::array_member(value& arr, zend_ulong idx)
	: arr_(arr)
	, idx_(idx)
	, pos_(HT_INVALID_IDX)
	, key_() {
		
	}
	array_member::array_member(value& arr, HashPosition pos)
	: arr_(arr)
	, idx_(-1)
	, pos_(pos)
	, key_() {

	}
	array_member& array_member::operator =(const value& val) {
		SEPARATE_ARRAY(static_cast<zval*>(arr_));
		if(idx_ != -1) {
			zend_hash_index_update(arr_, idx_, val);
			val.addref();
		}else if(pos_ != HT_INVALID_IDX) {
			zval* cur = zend_hash_get_current_data_ex(arr_, const_cast<HashPosition*>(&pos_)), tmp;
			zval_ptr_dtor(cur);
			ZVAL_COPY(cur, static_cast<zval*>(val));
		}else{
			zend_symtable_update(arr_, key_, val);
			val.addref();
		}
		// 直接将引用移动到数组中, 此处无需 addref() 引用调整
		return *this;
	}
	bool array_member::exists() const {
		if(idx_ != -1) {
			return zend_hash_index_exists(arr_, idx_);
		}else if(pos_ != HT_INVALID_IDX) {
			return true;
		}else{
			return zend_hash_exists(arr_, key_);
		}
	}
	// ---------------------------------------------------------
	array_member::operator value() const {
		return value(raw());
	}
	value array_member::ptr() const {
		return value(raw(), true);
	}
	zval* array_member::raw() const {
		if(idx_ != -1) {
			return zend_hash_index_find(arr_, idx_);
		}else if(pos_ != HT_INVALID_IDX) {
			return zend_hash_get_current_data_ex(arr_, const_cast<HashPosition*>(&pos_));
		}else{
			return zend_symtable_find_ind(arr_, key_);
		}
	}
}
