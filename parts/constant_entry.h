#pragma once

namespace php {
	class constant_entry {
	public:
		constant_entry(const php::string& name, const php::value& v);

		void declare(int module);
		void declare(zend_class_entry* ce);
	private:
		php::string key_;
		value       val_;
	};
}
