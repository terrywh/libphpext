#pragma once

#include "../types/value.h"

namespace php
{

class property_entry {
public:
	enum {
		PUBLIC    = ZEND_ACC_PUBLIC,
		PROTECTED = ZEND_ACC_PROTECTED,
		PRIVATE   = ZEND_ACC_PRIVATE,
	};
	// 常量须使用 持久 persistent 类型值 value（字符串、数组）
	property_entry(const std::string& name, const value& val, int access = PUBLIC);
	void declare(zend_class_entry* entry);

private:
	zend_string* name_;
	value        value_;
	int          access_;
};

}
