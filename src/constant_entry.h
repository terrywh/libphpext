#pragma once

namespace php {
	class constant_entry {
	public:
		constant_entry(const php::string& k, const php::value& v);
		void declare(int module);
		void declare(zend_class_entry* ce);
	private:
		string key_;
		value  val_;
	};
}
