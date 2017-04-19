#include "../phpext.h"
#include <typeinfo>
#include <iostream>

// class promise: public php::class_base {
// public:
// 	promise()
// 	: status_(0)
// 	, value_(nullptr) {}
//
// 	php::value __construct(php::parameters& params) {
// 		if(params.length() > 0 && params.is_callable())	{
// 			params[0]([this] (php::parameters& params) -> php::value {
// 				status_ = 1;
// 				value = params[0];
// 				return nullptr;
// 			}, [this] (php::parameters& params) -> php::value {
// 				status_ = -1;
// 				value= params[1];
// 				return nullptr;
// 			});
// 		}
// 		return nullptr;
// 	}
// 	bool status() const {
// 		return status_;
// 	};
// 	php::value& value() {
// 		return value_;
// 	}
// private:
// 	int        status_;
// 	php::value value_;
// };

// php::value test_function_2(php::parameters& params) {
// 	php::value fn(params[0]);
// 	return fn("abc");
// }
//
// php::value test_function_3(php::parameters& params) {
// 	// 支持引用方式传参（需要加入参数说明）
// 	params[0] = 1234;
// 	return 12 * 25;
// }
//
class test_class_1: public php::class_base {
public:
	php::value method_1(php::parameters& params) {
		if(params.length() > 0)	return params[0];
		return "abcdefg";
		// return nullptr;
	}
	php::value method_2(php::parameters& params) {
		return prop("property_1");
	}
	php::value method_3(php::parameters& params) {
		params[0] = 123456;
		return 123456;
	}
};

extern "C" {
	ZEND_DLEXPORT zend_module_entry* get_module() {
		static php::extension_entry extension(EXTENSION_NAME, EXTENSION_VERSION);
		// extension.add(php::ini_entry("phpext.ini_key_1", 111111));
		// extension.add(php::ini_entry("phpext.ini_key_2", "ini_value_2"));
		// extension.add(php::constant_entry("PHPEXT_CONSTANT_1", 222222));
		// extension.add(php::constant_entry("PHPEXT_CONSTANT_2", "constant_value_2"));

		extension.add<run>("lumos\\run");
		// extension.add<test_function_2>("phpext_function_2");
		// extension.add<test_function_3>("phpext_function_3", {
		// 	php::of_string("arg_1", true),
		// });
		// 内部 class
		// php::class_entry<php::class_closure> class_closure_entry("php\\__closure");
		// class_closure_entry.add<&php::class_closure::__invoke>("__invoke");
		// extension.add(std::move(class_closure_entry));
		php::class_entry<test_class_1> class_promise_entry("lumos\\promise");
		// class_promise_entry.add(php::constant_entry("CONSTANT_1", 333333));
		// class_promise_entry.add(php::property_entry("property_1", 123456));
		// class_promise_entry.add(php::property_entry("property_2", 123456, ZEND_ACC_PRIVATE));
		// class_promise_entry.add<&promise::method_1>("method_1");
		// class_promise_entry.add<&promise::method_2>("method_2");
		// class_promise_entry.add<&promise::method_3>("method_3", {
		// 	php::of_string("arg_1", true),
		// });
		extension.add(std::move(class_promise_entry));
		return extension;
	}
}
