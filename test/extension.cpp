#include "../src/phpext.h"

php::value test_function_1(php::parameters& params) {
	// php::callable fn = params[0];
	return "abc";
}

php::value test_function_2(php::parameters& params) {
	php::value fn(params[0]);
	return fn("abc");
}

php::value test_function_3(php::parameters& params) {
	// 支持引用方式传参（需要加入参数说明）
	params[0] = 1234;
	return 12 * 25;
}

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
		extension.add(php::ini_entry("phpext.ini_key_1", 11111));
		extension.add(php::ini_entry("phpext.ini_key_2", "ini_value_2"));
		extension.add(php::constant_entry("PHPEXT_CONSTANT_1", 22222));
		extension.add(php::constant_entry("PHPEXT_CONSTANT_2", "constant_value_2"));

		extension.add<test_function_1>("phpext_function_1");
		extension.add<test_function_2>("phpext_function_2");
		extension.add<test_function_3>("phpext_function_3", php::arguments()._string("arg_1",true).done());

		php::class_entry<test_class_1> test_class_1_entry("phpext_class_1");
		test_class_1_entry.add(php::property_entry("property_1", 123456));
		test_class_1_entry.add(php::property_entry("property_2", 123456, ZEND_ACC_PRIVATE));
		test_class_1_entry.add<&test_class_1::method_1>("method_1");
		test_class_1_entry.add<&test_class_1::method_2>("method_2");
		test_class_1_entry.add<&test_class_1::method_3>("method_3", php::arguments()._string("arg_1", true).done());
		extension.add(std::move(test_class_1_entry));
		return extension;
	}
}
