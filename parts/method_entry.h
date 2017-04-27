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
			value& rv = *reinterpret_cast<value*>(return_value);
			try {
				rv = (class_wrapper<T>::from_this(getThis())->*FUNCTION)(params);
			}catch(const exception& e) {
				zend_throw_exception(class_entry<class_exception>::entry(), e.what(), e.code());
			}catch(const std::exception& e){
				zend_throw_exception(class_entry<class_exception>::entry(), e.what(), 0);
			}catch(...){
				zend_throw_exception(class_entry<class_exception>::entry(), "unknown exception", 0);
			};
			ZVAL_COPY(return_value, (zval*)&rv);
		}
	};
}
