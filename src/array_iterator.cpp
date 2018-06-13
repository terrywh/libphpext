#include "phpext.h"

namespace php {
	void array_iterator::create() {
		zval key;
		zend_hash_get_current_key_zval_ex(arr_, &key, &pos_);
		val_.reset(new value_type {
			value(&key),
			array_member(arr_, pos_),
		});
	}
	array_iterator::array_iterator(value& a, HashPosition p)
	: arr_(a)
	, pos_(p) {
		create();
	}
	array_iterator::array_iterator(const array_iterator& ai)
	: arr_(ai.arr_)
	, pos_(ai.pos_)
	, val_(ai.val_)
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
		return *val_;
	}
	array_iterator::value_type* array_iterator::operator->() {
		return val_.get();
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