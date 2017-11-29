#pragma once

namespace php {
	class property_entry {
	public:
		property_entry(const std::string& name, std::nullptr_t v, int access = ZEND_ACC_PUBLIC);
		property_entry(const std::string& name, zend_bool v, int access = ZEND_ACC_PUBLIC);
		property_entry(const std::string& name, int v, int access = ZEND_ACC_PUBLIC);
		property_entry(const std::string& name, std::int64_t v, int access = ZEND_ACC_PUBLIC);
		property_entry(const std::string& name, double v, int access = ZEND_ACC_PUBLIC);
		property_entry(const std::string& name, const std::string& v, int access = ZEND_ACC_PUBLIC);

		property_entry(property_entry&& entry);
		void declare(zend_class_entry* entry);
	private:
		std::string  name_;
		value        value_;
		int          access_;
	};
}
