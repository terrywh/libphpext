#pragma once

#include "../types/value.h"

namespace php {
	class ini_entry {
	public:
		ini_entry(const std::string& name, std::int64_t val);
		ini_entry(const std::string& name, int val);
		ini_entry(const std::string& name, double val);
		ini_entry(const std::string& name, bool val);
		ini_entry(const std::string& name, const std::string& val);
		// 在 zend_register_ini_entries 后才能销毁当前对象（entry 中的引用会出现问题）
		void fill(zend_ini_entry_def* entry);
	private:
		std::string name_;
		std::string value_;
	};
}
