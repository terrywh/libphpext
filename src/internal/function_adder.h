namespace phpext {
	namespace internal {

		class function_adder {
		public:
			// 添加函数
			// 模版函数需要在 .h 文件中实现
			template <void (*callback)(parameters& params)>
			function_adder& add(string&& name, std::initializer_list<arg_info> init = {}) {
				_add(_function_entries, std::move(name), return_void<callback>, std::move(init));
				return *this;
			};
			template <phpext::value (*callback)(parameters& params)>
			function_adder& add(string&& name, std::initializer_list<arg_info> init = {}) {
				_add(_function_entries, std::move(name), return_value<callback>, std::move(init));
				return *this;
			};
		protected:
			zend_function_entry* do_register_functions();
			// @ zend_function_entry 参考 Zend/zend_API.h:36
			std::vector<zend_function_entry> _function_entries;
			static void _add(std::vector<zend_function_entry>& entries, string&& name, void(*callback)(zend_execute_data* ed, zval* rv), std::initializer_list<arg_info>&& _args);
		private:
			// 模版函数需要在 .h 中实现，否则会找不到符号
			template<void (*callback)(parameters& params)>
			static void return_void(zend_execute_data* execute_data, zval* return_value) {
				parameters params;
				PHP_CORE_TRY
				{
					callback(params);
				}
				PHP_CORE_CATCH;
			}
			template<value (*callback)(parameters& params)>
			static void return_value(zend_execute_data* execute_data, zval* return_value) {
				parameters params;
				value rv;
				PHP_CORE_TRY {
					rv = callback(params);
				} PHP_CORE_CATCH;
				ZVAL_COPY(return_value, rv.rv());
			}
		};
	}
}