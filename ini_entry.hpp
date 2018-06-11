#pragma once

namespace php {
	class ini_entry {
	public:
		ini_entry(const php::string& name, const php::value& val)
		: key_(name)
		, val_(val) {
			val_.to_string();
		}
		// 在 zend_register_ini_entries 后才能销毁当前对象（entry 中的引用会出现问题）
		void fill(zend_ini_entry_def* entry) {
			std::memset(entry, 0, sizeof(zend_ini_entry_def));
			entry->name            = key_.c_str();
			entry->name_length     = key_.length();
			// 未实现 modify_handler 进行实时的数据映射
			entry->value           = val_.c_str();
			entry->value_length    = val_.length();
			entry->modifiable      = ZEND_INI_ALL;
		}
	private:
		php::string key_;
		php::string val_;
	};
}
