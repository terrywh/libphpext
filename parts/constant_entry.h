#pragma once

namespace php
{

class constant_entry {
public:
	constant_entry(const std::string& name, std::nullptr_t v);
	constant_entry(const std::string& name, bool v);
	constant_entry(const std::string& name, int v);
	constant_entry(const std::string& name, std::int64_t v);
	constant_entry(const std::string& name, double v);
	constant_entry(const std::string& name, const std::string& v);


	void declare(int module);
	void declare(zend_class_entry* ce);
private:
	std::string name_;
	value       value_;
};

}
