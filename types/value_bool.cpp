#include "value.h"

namespace php
{
	value::value(bool v)
	: val_(&value_)
	, ref_(false)
	{
		if(v) ZVAL_TRUE(val_);
		else ZVAL_FALSE(val_);
	}
	value& value::to_bool() {
		convert_to_boolean(val_);
		return *this;
	}
	value::operator bool()
	{
		if(Z_TYPE_P(val_) == IS_TRUE) return true;
		return false;
	}
}
