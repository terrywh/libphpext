#pragma once

namespace php {
	class constant_entry {
	public:
		constant_entry(const std::string& name, const php::value& v);

		void declare(int module);
		void declare(zend_class_entry* ce);
	private:
		std::string name_;
		value       value_;
	};
}
