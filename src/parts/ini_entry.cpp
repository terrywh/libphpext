#include "ini_entry.h"

namespace php {
	ini_entry::ini_entry(const std::string& name, bool val)
	:name_(name), value_(val ? "yes" : "no") {}
	ini_entry::ini_entry(const std::string& name, std::int64_t val)
	:name_(name), value_(std::to_string(val)) {}
	ini_entry::ini_entry(const std::string& name, int val)
	:name_(name), value_(std::to_string(val)) {}
	ini_entry::ini_entry(const std::string& name, double val)
	:name_(name), value_(std::to_string(val)) {}
	ini_entry::ini_entry(const std::string& name, const std::string& val)
	:name_(name), value_(val) {}

	void ini_entry::fill(zend_ini_entry_def* entry) {
		entry->name            = name_.c_str();
		entry->name_length     = name_.length();
		entry->on_modify       = nullptr;
		entry->mh_arg1         = nullptr;
		entry->mh_arg2         = nullptr;
		entry->mh_arg3         = nullptr;
		entry->value           = value_.c_str();
		entry->value_length    = value_.length();
		entry->displayer       = nullptr;
		entry->modifiable      = ZEND_INI_ALL;
	}
}
