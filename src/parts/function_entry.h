#pragma once

#include "../vendor.h"
#include "../types/value.h"
#include "../types/parameters.h"
#include "../types/exception.h"

namespace php {
	class function_entry {
	public:
		function_entry(const std::string& name, value (*handler)(parameters& params));
		void fill(zend_function_entry* entry);
	private:
		std::string name_;
		value (*handler_)(parameters& params);

		// 实际函数包装
		template<value (*handler)(parameters& params)>
		static void function_handler(zend_execute_data* execute_data, zval* return_value) {
			parameters params(execute_data);
			value rv(return_value,/*ref=*/true);
			try {
				rv = handler(params);
			}catch(const exception& e) {
				zend_throw_exception(zend_ce_exception, e.what(), e.code());
			}catch(const std::exception& e){
				zend_throw_exception(zend_ce_exception, e.what(), -1);
			}catch(...){
				zend_throw_exception(zend_ce_exception, "unknown exception", -1);
			};
		}
	};
}
