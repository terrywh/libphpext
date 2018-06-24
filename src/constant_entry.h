#pragma once

#include "value.h"
#include "string.h"

namespace php {
	class constant_entry {
	public:
		constant_entry(const string& k, const value& v);
		void declare(int module);
		void declare(zend_class_entry* ce);
	private:
		string key_;
		value  val_;
	};
}
