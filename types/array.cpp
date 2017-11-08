#include "../phpext.h"

namespace php {
	array::array():value() {}
	array::array(std::size_t size) {
		ZVAL_NEW_ARR(&value_);
		_zend_hash_init(Z_ARRVAL(value_), size, ZVAL_PTR_DTOR, 0 ZEND_FILE_LINE_RELAY_CC);
	}
	bool array::is_a_list() {
		return length() > 0 && begin()->first.is_long();
	}
	bool array::is_a_map() {
		return length() > 0 && begin()->first.is_string();
	}
	array& array::operator = (const array& arr) {
		_zval_dtor(&value_);
		ZVAL_COPY(&value_, &arr.value_);
	}
	array& array::operator = (array&& arr) {
		_zval_dtor(&value_);
		ZVAL_COPY_VALUE(&value_, &arr.value_);
		ZVAL_UNDEF(&arr.value_);
	}
	array_iterator array::begin() {
		return std::move(array_iterator(*this));
	}
	array_iterator array::end() {
		return std::move(array_iterator(*this, Z_ARRVAL(value_)->nNumUsed));
	}

	array_iterator& array_iterator::operator++() {
		assert(pos_ < end_);   // throw
		zval* z = nullptr;
		do {
			z = &(buk_ + ++pos_)->val;
		} while(Z_TYPE_P(z) == IS_UNDEF && pos_ < end_);
		return *this;
	}
	array_iterator  array_iterator::operator++(int) {
		assert(pos_ < end_);
		array_iterator ai = *this;
		zval* z = nullptr;
		do {
			z = &(buk_ + ++pos_)->val;
		} while(Z_TYPE_P(z) == IS_UNDEF && pos_ < end_);
		return ai;
	}
	array_iterator& array_iterator::operator--() {
		assert(pos_ > 0);   // throw
		//if(pos_ == 0) return *this;
		zval* z = nullptr;
		do {
			z = &(buk_ + --pos_)->val;
		} while(Z_TYPE_P(z) == IS_UNDEF && pos_ > 0);
		return *this;
	}
	array_iterator  array_iterator::operator--(int) {
		assert(pos_ > 0);
		array_iterator ai = *this;
		zval* z = nullptr;
		do {
			z = &(buk_ + --pos_)->val;
		} while(Z_TYPE_P(z) == IS_UNDEF && pos_ > 0);
		return ai;
	}

	array_iterator::reference array_iterator::operator*() {
		Bucket* bt = buk_ + pos_;
		return val_ = value_type(
			bt->key ? php::value(bt->key) : php::value(bt->h),
			bt->val);
	}
	array_iterator::pointer   array_iterator::operator->() {
		Bucket* bt = buk_ + pos_;
		return &(val_ = value_type(
			bt->key ? php::value(bt->key) : php::value(bt->h),
			bt->val));
	}
	bool operator==(const array_iterator& lhs, const array_iterator& rhs) {
		if(lhs.buk_ == rhs.buk_ && lhs.pos_ == rhs.pos_) {
			return true;
		}
		return false;
	}
	bool operator!=(const array_iterator& lhs, const array_iterator& rhs) {
		if(lhs.buk_ != rhs.buk_ || lhs.pos_ != rhs.pos_) {
			return true;
		}
		return false;
	}
}
