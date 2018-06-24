#pragma once

#include "string.h"

namespace php {
	class ini_entry {
	public:
		ini_entry(const string& name, const value& val);		
		void fill(zend_ini_entry_def* entry);
	private:
		string key_;
		string val_;
	};
}
