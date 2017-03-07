#include "../phpext.h"

namespace php {
	ini_entry::ini_entry(const std::string& name, bool val)
	: name_(name)
	, value_(val ? "yes" : "no") {}
	ini_entry::ini_entry(const std::string& name, std::int64_t val)
	: name_(name)
	, value_(std::to_string(val)) {}
	ini_entry::ini_entry(const std::string& name, int val)
	: name_(name)
	, value_(std::to_string(val)) {}
	ini_entry::ini_entry(const std::string& name, double val)
	: name_(name)
	, value_(std::to_string(val)) {}
	ini_entry::ini_entry(const std::string& name, const char* val)
	: name_(name)
	, value_(val) {}
	ini_entry::ini_entry(const std::string& name, const std::string& val)
	: name_(name)
	, value_(val) {}

	static void displayer_handler(zend_ini_entry *entry, int type) {
		if (type == ZEND_INI_DISPLAY_ORIG && entry->modified && entry->orig_value) {
			zend_write(entry->orig_value->val, entry->orig_value->len);
		}else if(entry->value) {
			zend_write(entry->value->val, entry->value->len);
		}else {
		 	zend_write("null", 4);
		}
	}

	void ini_entry::fill(zend_ini_entry_def* entry) {
		entry->name            = name_.c_str();
		entry->name_length     = name_.length();
		// TODO ？ 实现 modify_handler 进行实时的数据映射
		entry->on_modify       = nullptr;
		entry->mh_arg1         = nullptr;
		entry->mh_arg2         = nullptr;
		entry->mh_arg3         = nullptr;
		entry->value           = value_.c_str();
		entry->value_length    = value_.length();
		entry->displayer       = displayer_handler;
		// TODO 由于未实现 modify_handler 故这暂时仅允许系统ini设置
		entry->modifiable      = ZEND_INI_SYSTEM;
	}
}
