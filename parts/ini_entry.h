#pragma once

namespace php {
	class ini_entry {
	public:
		ini_entry(const std::string& name, const php::value& val);
		// 在 zend_register_ini_entries 后才能销毁当前对象（entry 中的引用会出现问题）
		void fill(zend_ini_entry_def* entry);
	private:
		php::string key_;
		php::string val_;
	};
}
