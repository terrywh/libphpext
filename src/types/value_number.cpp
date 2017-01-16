#include "value.h"

namespace php
{
	// 整数
	value::value(int v)
	:value() {
		ZVAL_LONG(val_, v);
	}
	value::value(std::int64_t v)
	:value() {
		ZVAL_LONG(val_, v);
	}
	value& value::to_long(int base) {
		convert_to_long_base(val_, base);
		return *this;
	}

	value::operator int() {
		assert(Z_TYPE_P(val_) == IS_LONG);
		return Z_LVAL_P(val_);
	}
	value::operator std::int64_t() {
		assert(Z_TYPE_P(val_) == IS_LONG);
		return Z_LVAL_P(val_);
	}
	value& value::operator= (int i) {
		_zval_dtor(val_);
		ZVAL_LONG(val_, i);
	}
	value& value::operator= (std::int64_t l) {
		_zval_dtor(val_);
		ZVAL_LONG(val_, l);
	}
	// 浮点
	value::value(double d)
	:value()
	{
		ZVAL_DOUBLE(val_, d);
	}
	value& value::to_double() {
		convert_to_double(val_);
		return *this;
	}
	value::operator double() {
		assert(Z_TYPE_P(val_) == IS_DOUBLE);
		return Z_DVAL_P(val_);
	}
	value& value::operator= (double d) {
		_zval_dtor(val_);
		ZVAL_DOUBLE(val_, d);
	}
	// TODO 判定
	// TODO 运算

}
