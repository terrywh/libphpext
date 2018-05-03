#include "../phpext.h"

namespace php {
	array_iterator array::begin() const {
		array_iterator i(*this, -1);
		++i; // 防止首个元素 IS_UNDEF 的情况
		return std::move(i);
	}
	const array_iterator array::end() const {
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
