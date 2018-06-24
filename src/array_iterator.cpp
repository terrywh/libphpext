#include "vendor.h"
#include "array_iterator.h"

namespace php {
	void array_iterator::create() {
		value_type* vt = new value_type {
			value(), // 不允许更新 KEY
			value(zend_hash_get_current_data_ex(arr_, &pos_), true) // 允许更新 VAL
		};
		zend_hash_get_current_key_zval_ex(arr_, vt->first, &pos_); // 内部 ZVAL_STR_COPY
		entry_.reset(vt);
	}
	array_iterator::array_iterator(value& a, HashPosition p)
	: arr_(a)
	, pos_(p) {
		create();
	}
	array_iterator::array_iterator(const array_iterator& ai)
	: arr_(ai.arr_)
	, pos_(ai.pos_)
	, entry_(ai.entry_)
	{

	}
	array_iterator& array_iterator::operator++() {
		zend_hash_move_forward_ex(arr_, &pos_);
		create();
		return *this;
	}
	array_iterator  array_iterator::operator++(int) {
		array_iterator ai = *this;
		++ai;
		return ai;
	}
	array_iterator& array_iterator::operator--() {
		zend_hash_move_backwards_ex(arr_, &pos_);
		create();
		return *this;
	}
	array_iterator array_iterator::operator--(int) {
		array_iterator ai = *this;
		--ai;
		return ai;
	}
	array_iterator::value_type& array_iterator::operator*() {
		return *entry_;
	}
	array_iterator::value_type* array_iterator::operator->() {
		return entry_.get();
	}
	array_iterator array_iterator::operator+(size_t n)  {
		array_iterator tmp(*this);
		while(n--) ++tmp;
		return tmp;
	}
	array_iterator& array_iterator::operator+=(size_t n) {
		while(n--) ++(*this);
		return *this;
	}
	array_iterator array_iterator::operator-(size_t n)  {
		array_iterator tmp(*this);
		while(n--) --tmp;
		return tmp;
	}
	array_iterator& array_iterator::operator-=(size_t n) {
		while(n--) --(*this);
		return *this;
	}
	bool array_iterator::operator==(const array_iterator& ai) const {
		if(arr_ == ai.arr_ && pos_ == ai.pos_) {
			return true;
		}
		return false;
	}
	bool array_iterator::operator!=(const array_iterator& ai) const {
		if(arr_ != ai.arr_ || pos_ != ai.pos_) {
			return true;
		}
		return false;
	}
}