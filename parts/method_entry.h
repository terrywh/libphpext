#pragma once
#include "../vendor.h"
#include "../types/value.h"
#include "../types/parameters.h"
#include "../types/exception.h"
#include "arguments.h"

namespace php {
	template <class T, value (T::*FUNCTION)(parameters& params)>
	class method_entry {
	public:
		void static fill(zend_function_entry* entry, const char* name, int access) {
			entry->fname   = name;
			entry->handler = method_entry<T, FUNCTION>::method_delegate;
			entry->arg_info = nullptr;
			entry->num_args = 0;
			entry->flags    = access;
		}
		void static fill(zend_function_entry* entry, const char* name, const arguments& info, int access) {
			entry->fname   = name;
			entry->handler = method_entry<T, FUNCTION>::method_delegate;
			entry->arg_info = static_cast<zend_internal_arg_info*>(info);
			entry->num_args = (std::uint32_t)info.length();
			entry->flags    = access;
		}

	private:
		// 实际函数包装
		static void method_delegate(zend_execute_data* execute_data, zval* return_value) {
			parameters params(execute_data);
			value rv(nullptr);
			try {
				rv = (class_wrapper<T>::from_this(getThis())->*FUNCTION)(params);
			}catch(const exception& e) {
				zend_throw_exception(zend_ce_exception, e.what(), e.code());
			}catch(const std::exception& e){
				zend_throw_exception(zend_ce_exception, e.what(), -1);
			}catch(...){
				zend_throw_exception(zend_ce_exception, "unknown exception", -1);
			};
			ZVAL_COPY(return_value, rv.data());
		}
	};
}
