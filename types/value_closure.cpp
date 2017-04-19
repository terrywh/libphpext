#include "../phpext.h"

namespace php
{
	value::value(std::function<value (parameters&)> fn)
	: val_(&value_)
	, ref_(false) {
		zend_object* obj = class_entry<class_closure>::create_object();
		class_closure* cc = class_wrapper<class_closure>::from_obj(obj);
		cc->fn_ = fn;
		ZVAL_OBJ(val_, obj);
	}

}
