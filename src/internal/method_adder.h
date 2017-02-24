namespace phpext {
	namespace internal {
		template <class T>
		class method_adder {
		public:
			inline static T* from_this(zval* This) {
				return class_object<T>::from_object(Z_OBJ_P(This))->cpp;
			};
			template <void (T::*callback)(parameters& params)>
			static void return_void(zend_execute_data* execute_data, zval* return_value) {
				parameters params;
				PHP_CORE_TRY {
					(from_this(getThis())->*callback)(params);
				} PHP_CORE_CATCH;
			};
			template <value (T::*callback)(parameters& params)>
			static void return_value(zend_execute_data* execute_data, zval* return_value) {
				parameters params;
				PHP_CORE_TRY {
					(from_this(getThis())->*callback)(params);
				} PHP_CORE_CATCH;
			};
		};
		
	
		// class method_adder:public function_adder {
		// public:
			
		// 	// template <class T, bool (T::*callback)(parameters& params)>
		// 	// method_adder& add(string&& name, std::initializer_list<arg_info> init = {}) {
		// 	// 	function_adder::_add(_function_entries, std::move(name), internal::function_implement_bool<callback>, std::move(init));
		// 	// 	return *this;
		// 	// };
		// 	// template <class T, std::int64_t (T::*callback)(parameters& params)>
		// 	// method_adder& add(string&& name, std::initializer_list<arg_info> init = {}) {
		// 	// 	function_adder::_add(_function_entries, std::move(name), internal::function_implement_int64<callback>, std::move(init));
		// 	// 	return *this;
		// 	// };
		// 	// template <class T, double (T::*callback)(parameters& params)>
		// 	// method_adder& add(string&& name, std::initializer_list<arg_info> init = {}) {
		// 	// 	function_adder::_add(_function_entries, std::move(name), internal::function_implement_double<callback>, std::move(init));
		// 	// 	return *this;
		// 	// };
		// 	// template <class T, phpext::string (T::*callback)(parameters& params)>
		// 	// method_adder& add(string&& name, std::initializer_list<arg_info> init = {}) {
		// 	// 	function_adder::_add(_function_entries, std::move(name), internal::function_implement_string<callback>, std::move(init));
		// 	// 	return *this;
		// 	// };
		// 	// template <class T, phpext::value (T::*callback)(parameters& params)>
		// 	// method_adder& add(string&& name, std::initializer_list<arg_info> init = {}) {
		// 	// 	function_adder::_add(_function_entries, std::move(name), internal::function_implement_value<callback>, std::move(init));
		// 	// 	return *this;
		// 	// };
		// };
	}
}