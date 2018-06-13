#pragma once

namespace php {
	class ini_entry {
	public:
		ini_entry(const php::string& name, const php::value& val);		
		void fill(zend_ini_entry_def* entry);
	private:
		php::string key_;
		php::string val_;
	};
}
