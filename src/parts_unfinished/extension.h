#pragma once

namespace phpext {
	class extension: public internal::function_adder, public internal::ini_adder, public internal::constant_adder {
		public:
			extension(const char* name, const char* version);
			operator zend_module_entry*();
			// 添加一个 class
			extension& add(class_entry_base&& entry);
			// 使用 function_ctr 中对应的 add 方法
			using internal::ini_adder::add;
			using internal::function_adder::add;
			using internal::constant_adder::add;
		private:
			static inline extension& get();
			static extension*                _extension;
			zend_module_entry                _entry;   // 扩展描述
			int                              _module;

			std::vector<class_entry_base> _classes;

	};

}
