#include "../phpext.h"

namespace php
{
	// 整数
	value::value(int v)
	: val_(&value_)
	, ref_(false) {
		ZVAL_LONG(val_, v);
	}
	value::value(std::int64_t v)
	: val_(&value_)
	, ref_(false) {
		ZVAL_LONG(val_, v);
	}
	value& value::to_long(int base) {
		convert_to_long_base(val_, base);
		return *this;
	}

	value::operator int() {
		if( !is_long() ) throw exception("type error: long expected");
		return Z_LVAL_P(val_);
	}
	value::operator std::int64_t() {
		if( !is_long() ) throw exception("type error: long expected");
		return Z_LVAL_P(val_);
	}
	// bool value::operator==(int i) const {
	// 	return ref_ && val_ == nullptr || Z_TYPE_P(val_) != IS_LONG ? false : Z_LVAL_P(val_) == i;
	// }
	// bool value::operator==(std::int64_t l) const {
	// 	return ref_ && val_ == nullptr || Z_TYPE_P(val_) != IS_LONG ? false : Z_LVAL_P(val_) == l;
	// }
	// bool value::operator>(int i) const {
	// 	if( is_long() ) throw exception("type error: long expected");
	// 	return Z_LVAL_P(val_) > i;
	// }
	// bool value::operator<(int i) const {
	// 	if( is_long() ) throw exception("type error: long expected");
	// 	return Z_LVAL_P(val_) < i;
	// }
	// bool value::operator>(std::int64_t l) const {
	// 	if( is_long() ) throw exception("type error: long expected");
	// 	return Z_LVAL_P(val_) > l;
	// }
	// bool value::operator<(std::int64_t l) const {
	// 	if( is_long() ) throw exception("type error: long expected");
	// 	return Z_LVAL_P(val_) < l;
	// }
	// 浮点
	value::value(double d)
	: val_(&value_)
	, ref_(false) {
		ZVAL_DOUBLE(val_, d);
	}
	value& value::to_double() {
		convert_to_double(val_);
		return *this;
	}
	value::operator double() {
		if( !is_double() ) throw exception("type error: double expected");
		return Z_DVAL_P(val_);
	}
	// bool value::operator==(double d) const {
	// 	return ref_ && val_ == nullptr || Z_TYPE_P(val_) != IS_DOUBLE ? false : Z_DVAL_P(val_) == d;
	// }
	// bool value::operator>(double d) const {
	// 	assert( is_double() );
	// 	return Z_DVAL_P(val_) > d;
	// }
	// bool value::operator<(double d) const {
	// 	assert( is_double() );
	// 	return Z_DVAL_P(val_) < d;
	// }
}
