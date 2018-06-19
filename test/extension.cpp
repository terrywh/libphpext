#include "../src/phpext.h"
#include <iostream>

// 所有导出到 PHP 的函数必须符合下面形式：
// php::value fn(php::parameters& params);
php::value test_function_1(php::parameters& params) {
	// 参数下标必须存在, 否则会抛出异常
	php::value v0 = params[0];
	// 除非在函数声明时明确类型, 否则应添加类型检查
	int v1;
	if(params[1].typeof(php::TYPE::INTEGER)) {
		v1 = static_cast<php::value>(params[1]);
	}
	// 强制类型转换
	double v2 = params[2].to_float();
	php::string v3 = params[3];
	// 构造指定长度
	php::string rv(256 + v3.length());
	// 在新字符串的缓冲区中构造一个新的字符串
	rv.shrink(sprintf(rv.data(), "[%d] [%d] [%f] [%s]", v0.to_boolean(), v1, v2, v3.c_str()));
	// PHP 数组
	php::array v4 = params[4];
	// copy on write
	v4["aaaaa"] = "1111111";
	v4["bbbbb"] = php::string("2222222", 7);
	// 遍历
	std::printf("\n");
	for(auto i=v4.begin(); i!=v4.end(); ++i) {
		php::string key = i->first;
		php::string val = i->second;
		std::printf("    key: %s => val: %s\n", key.c_str(), val.c_str());
	}
	return rv;
}
php::value test_function_2(php::parameters& params) {
	// 回调函数
	php::callable cb = params[0];
	// 回调函数调用传入 a 作为参数，并返回其返回值
	php::value rv;
	try{ // 捕获 PHP 用户代码异常
		rv = cb({params[1]});
	}catch(const php::exception& ex) {
		rv = ex;
	}
	// 不改变 PHP 自己的异常逻辑
	return rv;
}
php::value test_function_3(php::parameters& params) {
	// 函数声明了首个必要参数为“引用”传递，可以更改其值
	php::value v = params[0];
	params[0] = static_cast<int>(params[0]) * 2;
	// “无” 返回值，但对 C++ 必须要有返回值
	return nullptr;
}
php::value test_function_4(php::parameters& params) {
	// 参数类型限定
	php::object date1 = static_cast<php::value>(params[0]);
	php::callable cb2 = static_cast<php::value>(params[1]);
	// 调用对象方法
	date1.call("modify", {"+1day"});
	// 调用回调
	return cb2({php::json_encode(date1)});
}
php::value test_function_5(php::parameters& params) {
	// 返回一个 C++ 定义的回调函数
	// C++14
	// return [a1 = static_cast<php::value>(params[0])] (php::parameters& params) -> php::value {
		// return static_cast<int>(a1) + static_cast<int>(static_cast<php::value>(params[0]));
	// };
	// C++11
	php::value a1 = static_cast<php::value>(params[0]);
	return php::value([a1] (php::parameters& params) -> php::value {
		return static_cast<int>(a1) + static_cast<int>(static_cast<php::value>(params[0]));
	});
}
php::value test_function_6(php::parameters& params) {
	return nullptr;
}
// 
class test_class_1: public php::class_base {
public:
	// 属性操作方式 1
	php::value method_1(php::parameters& params) {
		// 读取 1.
		php::value p = get("property_1");
		get("property_1");
		// 更新 1. 
		set("property_1", params[0]);
		return nullptr;
	}
	// 属性操作方式 2
	php::value method_2(php::parameters& params) {
		(*this)["property_2"] = params[0];
		return nullptr;
	}
	php::value method_3(php::parameters& params) {

		return nullptr;
	}
};

extern "C" {
	ZEND_DLEXPORT zend_module_entry* get_module() {
		static php::extension_entry ext("phpext", "1.0");
		ext
			.desc({"DESC_1", "DESC_CONTENT_111111111111111111111111"})
			.desc({"DESC_2", "DESC_CONTENT_22222222222222222222"})
			.constant({"CONSTANT_1", 22222})
			.constant({"CONSTANT_2", "THIS_IS_EXTENSION_CONSTANT"})
			.ini({"config_1", "default_1"})
			.ini({"config_2", "default_2"})
			.function<test_function_1>("test_function_1")
			.function<test_function_2>("test_function_2")
			.function<test_function_3>("test_function_3", {
				// 引用传递一个必要参数
				{"arg1", php::TYPE::INTEGER, true},
			})
			.function<test_function_4>("test_function_4", {
				{"arg1", "DateTime"}, // 类
				{"arg2", php::TYPE::CALLABLE}, // Callable 被“强化”确认正确性
			})
			.function<test_function_5>("test_function_5")
			.function<test_function_6>("test_function_6");

		php::class_entry<test_class_1> class_test_1("test_class_1");
		class_test_1.constant({"CONSTANT_1", 333333});
		class_test_1.constant({"CONSTANT_1", "THIS_IS_CLASS_CONSTANT"});
		class_test_1.property({"property_1", 123456});
		class_test_1.property({"property_2", nullptr});
		class_test_1.method<&test_class_1::method_1>("method_1");
		class_test_1.method<&test_class_1::method_2>("method_2");
		class_test_1.method<&test_class_1::method_3>("method_3");
		ext.add(std::move(class_test_1));

		return ext;
	}
};

