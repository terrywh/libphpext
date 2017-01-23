#pragma once

#include "../types/value.h"
#include "../types/parameters.h"
#include "ini_entry.h"
#include "constant_entry.h"
#include "../types/exception.h"

namespace php {
	class extension_entry {
	public:
		extension_entry(const char* name, const char* version);

		extension_entry& add(const ini_entry& entry);
		extension_entry& add(const constant_entry& entry);
		// 函数
		template <value (*function)(parameters& params)>
		extension_entry& add(const std::string& name) {
			return *this;
		}
		// 类
		// extension& add(class_entry&& entry);
		int module_number;
	private:
		std::vector<ini_entry>           ini_entries;
		std::vector<constant_entry>      constant_entries;
		std::vector<zend_function_entry> function_entries;
		// 实际函数包装
		template<value (*function)(parameters& params)>
		static void function_handler(zend_execute_data* execute_data, zval* return_value) {
			parameters params(execute_data);
			value rv(return_value,/*ref=*/true);
			try {
				rv = function(params);
			}catch(const exception& e) {
				zend_throw_exception(zend_ce_exception, e.what(), e.code());      \
				return;                                                           \
			}catch(const std::exception& e){                                      \
				zend_throw_exception(zend_ce_exception, e.what(), -1);             \
				return;                                                           \
			}catch(...){                                                          \
				zend_throw_exception(zend_ce_exception, "unknown exception", -1); \
				return;                                                           \
			};
		}
		// 扩展回调函数
		static int on_module_startup  (int type, int module);
		static int on_module_shutdown (int type, int module);
		static int on_request_startup (int type, int module);
		static int on_request_shutdown(int type, int module);

		static extension_entry* self_;
		zend_module_entry* entry_;
	};
}
