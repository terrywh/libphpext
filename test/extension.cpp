#include "../src/phpext.h"
#include <iostream>

// 所有导出到 PHP 的函数必须符合下面形式：
// php::value fn(php::parameters& params);
php::value test_function_1(php::parameters& params) {
	// 参数下标必须存在, 否则会抛出异常
	php::value v0 = params[0];
	// 除非在函数声明时明确类型, 否则应添加类型检查
	int v1;
	if(params[1].type_of(php::TYPE::INTEGER)) {
		v1 = params[1];
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
		i->second.to_string();
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

	params[0] = static_cast<int>(params[0]) * 2;
	php::value v = 654321;
	v = 123456;
	// “无” 返回值，但对 C++ 必须要有返回值
	return nullptr;
}
php::value test_function_4(php::parameters& params) {
	// 参数类型限定
	php::object date1 = params[0];
	php::callable cb2 = params[1];
	// 调用对象方法
	date1.call("modify", {"+1day"});
	// 调用回调
	try{
		cb2.call();
	}
	catch(const php::exception& ex)
	{
		std::cout << "exception caught!\n";

	}
	return nullptr;
}
php::value test_function_5(php::parameters& params) {
	// C++11
	php::value a1 = params[0];
	return php::value([a1] (php::parameters& params) -> php::value {
		return static_cast<int>(a1) + static_cast<int>(params[0]);
	});
}
php::value test_function_6(php::parameters& params) {
	php::stream_buffer sb;
	std::ostream os(&sb);
	os << php::exception("exception");
	for(int i = 0; i< params.length(); ++i) {
		os << " " << params[i].ptr();
	}
	return std::move(sb); // php::value 移动构造
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
		return php::array(8);
	}
};

extern "C" {
	ZEND_DLEXPORT zend_module_entry* get_module() {
		static php::extension_entry ext("phpext", "1.0");
		// !!! 内部 class 需要使用 php::value([] (php::parameters& params) -> php::value {}); 函数须要
		php::class_entry<php::closure> class_closure("ext_closure");
		class_closure.method<&php::closure::__invoke>("__invoke");
		ext.add(std::move(class_closure));

		ext
			.desc({"DESC_1", "DESC_CONTENT_111111111111111111111111"})
			.desc({"DESC_2", "DESC_CONTENT_22222222222222222222"})
			.constant({"CONSTANT_1", 22222})
			.constant({"CONSTANT_2", "THIS_IS_EXTENSION_CONSTANT"})
			.ini({"config_1", "default_1"})
			.ini({"config_2", "default_2"})
			.function<test_function_1>("test_function_1", {
				{"boolean", php::TYPE::BOOLEAN}, // 基础类型无法强化检查 (用户代码需要自行检查)
			})
			.function<test_function_2>("test_function_2")
			.function<test_function_3>("test_function_3", {
				// 引用传递一个必要参数
				{"arg1", php::TYPE::INTEGER, true},
			})
			.function<test_function_4>("test_function_4", {
				{"arg1", "DateTime"}, // 类 被"强化"确认类型正确
				{"arg2", php::TYPE::CALLABLE}, // Callable 被“强化”确认类型正确
			})
			.function<test_function_5>("test_function_5")
			.function<test_function_6>("test_function_6");

		// php::class_entry<test_class_1> class_test_1("test_class_1");
		// class_test_1.constant({"CONSTANT_1", 333333});
		// class_test_1.constant({"CONSTANT_1", "THIS_IS_CLASS_CONSTANT"});
		// class_test_1.property({"property_1", 123456});
		// class_test_1.property({"property_2", nullptr});
		// class_test_1.method<&test_class_1::method_1>("method_1");
		// class_test_1.method<&test_class_1::method_2>("method_2");
		// class_test_1.method<&test_class_1::method_3>("method_3");
		// ext.add(std::move(class_test_1));

		return ext;
	}
};
