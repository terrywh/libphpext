#pragma once

namespace php {
	template <value FUNCTION(parameters& params)>
	class function_entry {
	public:
		static void fill(zend_function_entry* entry, const char* name, int flags = 0) {
			entry->fname   = name;
			entry->handler = function_entry<FUNCTION>::function_delegate;
			entry->arg_info = nullptr;
			entry->num_args = 0;
			entry->flags    = flags;
		}
		static void fill(zend_function_entry* entry, const char* name, const arguments& argi, int flags = 0) {
			entry->fname   = name;
			entry->handler = function_entry<FUNCTION>::function_delegate;
			entry->arg_info = static_cast<zend_internal_arg_info*>(argi);
			entry->num_args = (std::uint32_t)argi.length();
			entry->flags    = flags;
		}
	private:
		static void function_delegate(zend_execute_data* execute_data, zval* return_value) {
			parameters params(execute_data);
			value& rv = *reinterpret_cast<value*>(return_value);
			try {
				rv = FUNCTION(params);
			}catch(const exception& e) {
				zval ex;
				ZVAL_COPY(&ex, e);
			 	zend_throw_exception_object(&ex);
			}catch(const std::exception& e){
				zend_throw_exception(nullptr, e.what(), 0);
			}catch(...){
				zend_throw_exception(nullptr, "unknown exception", 0);
			};
		}
	};
}
