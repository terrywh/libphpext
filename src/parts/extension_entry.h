#pragma once

#include "../types/value.h"
#include "../types/parameters.h"
#include "../types/exception.h"

#include "ini_entry.h"
#include "constant_entry.h"
#include "function_entry.h"

namespace php {
	class extension_entry {
	public:
		extension_entry(const char* name, const char* version);
		~extension_entry();
		
		extension_entry& add(const ini_entry& entry);
		extension_entry& add(const constant_entry& entry);
		// 函数
		extension_entry& add(const function_entry& entry);
		// 类
		// extension& add(class_entry&& entry);
		int module_number;

		operator zend_module_entry*();
	private:
		std::vector<ini_entry>           ini_entries_;
		std::vector<constant_entry>      constant_entries_;
		std::vector<function_entry>      function_entries_;

		// 扩展回调函数
		static int on_module_startup  (int type, int module);
		static int on_module_shutdown (int type, int module);
		static int on_request_startup (int type, int module);
		static int on_request_shutdown(int type, int module);

		static extension_entry* self_;
		zend_module_entry* entry_;
	};
}
