#include "value.h"

namespace php
{
	value::value(bool v)
	:value()
	{
		if(v) ZVAL_TRUE(val_);
		else ZVAL_FALSE(val_);
	}
	value::operator bool()
	{
		if(Z_TYPE_P(val_) == IS_TRUE) return true;
		return false;
	}
	value& value::operator= (bool b)
	{
		_zval_dtor(val_);
		if(b) ZVAL_TRUE(val_);
		else ZVAL_FALSE(val_);
	}

}
