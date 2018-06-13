#pragma once

namespace php {
	class property_entry {
	public:
		property_entry(const php::string& name, const php::value& v, int access = PUBLIC);
		property_entry(property_entry&& entry);
		void declare(zend_class_entry* entry);
	private:
		string key_;
		value  val_;
		int    acc_;
	};
}
