#include "phpext.h"

namespace php {
	ini_entry::ini_entry(const php::string& name, const php::value& val)
	: key_(name)
	, val_(val) {
		val_.to_string();
	}
	void ini_entry::fill(zend_ini_entry_def* entry) {
		std::memset(entry, 0, sizeof(zend_ini_entry_def));
		entry->name            = key_.c_str();
		entry->name_length     = key_.length();
		// 未实现 modify_handler 进行实时的数据映射
		entry->value           = val_.c_str();
		entry->value_length    = val_.length();
		entry->modifiable      = ZEND_INI_ALL;
	}
}
