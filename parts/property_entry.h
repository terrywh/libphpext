#pragma once

namespace php
{

class property_entry {
public:
	// 常量须使用 持久 persistent 类型值 value（字符串、数组）
	property_entry(const std::string& name, value&& val, int access = ZEND_ACC_PUBLIC);
	void declare(zend_class_entry* entry);

	property_entry(property_entry&& entry);
private:
	zend_string* name_;
	value        value_;
	int          access_;
};

}
