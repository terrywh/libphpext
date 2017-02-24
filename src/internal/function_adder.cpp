#include "../../core.h"
namespace phpext {
	namespace internal {
		// @ zend_function_entry 参考 Zend/zend_API.h:36
		zend_function_entry* function_adder::do_register_functions() {
			// _fun_entries 尾部需要一个空定义
			_function_entries.push_back(*static_cast<zend_function_entry*>(internal::zero));
			return _function_entries.data();
		}
		void function_adder::_add(std::vector<zend_function_entry>& entries, string&& name, void(*callback)(zend_execute_data* ed, zval* rv), std::initializer_list<arg_info>&& _args) {
			name.addref(); // 不要回收
			zend_function_entry entry;
			entry.fname    = name.data();
			entry.handler  = callback;
			// @ zend_internal_arg_info 参考 Zend/zend_compile.h:300
			std::vector<zend_internal_arg_info>* args = new std::vector<zend_internal_arg_info>();
			int required = 0;
			for(auto i=_args.begin();i!=_args.end();++i) {
				if(!i->allow_null) ++required;
				args->emplace_back(*(zend_internal_arg_info*)i);
			}
			// 首个参数为固定描述必选参数、返回引用
			// { (const char*)(zend_uintptr_t)(required_num_args), NULL, 0, return_reference, 0, 0 }
			args->insert(args->begin(), zend_internal_arg_info{ // e
				(const char*)(zend_uintptr_t)(required), nullptr, 0, 0, false, false 
			});
			entry.arg_info = args->data();
			entry.num_args = args->size() - 1;
			entry.flags    = 0;
			entries.push_back(std::move(entry));
		}
	}
}