#include "../phpext.h"
#include <typeinfo>
#include <iostream>
// 所有导出到 PHP 的函数必须符合下面形式：
// php::value fn(php::parameters& params);
php::value test_function_1(php::parameters& params) {
	// PHP 字符串，从 params 中可以获得对象，也可以获取其 引用
	php::string& str = params[0];
	// 数值( php::value 类型可以通过 static_cast 转换为常见类型)
	int num = params[1];
	// 构造指定长度
	php::string buf(str.length() + 16);
	// 在 buf 新字符串的缓冲区中构造一个新的字符串
	buf.length() = sprintf(buf.data(), "[%.*s]:[%d]", str.length(), str.c_str(), num);
	return buf;
}

php::value test_function_2(php::parameters& params) {
	// PHP 数组
	php::array a(5);
	a["aaaaa"] = php::string("1111111");
	a["bbbbb"] = php::string("2222222");
	// 遍历
	for(auto i=a.begin();i!=a.end();++i) {
		// php::value 为通用数据类型，对应 zval 结构体
		php::value kkk = i->first;
		php::value vvv = i->second;
		// php::string/callable/object 为专用类型，php::value 的子类，也对应 zval 结构体
		// 转换过程会自动进行类型检查，不符合的类型会抛出异常到 PHP 中
		php::string key = i->first;
		php::string val = i->second;
		printf("\tkey: %s => val: %s\n", key.c_str(), val.c_str());
	}
	// 回调函数
	php::callable cb = params[0];
	// 回调函数调用传入 a 作为参数，并返回其返回值
	return cb(a);
}

php::value test_function_3(php::parameters& params) {
	// 函数声明了首个参数为“引用”传递，可以更改其值
	params[0] = 654321;
	// “无” 返回值，但对 C++ 必须要有返回值
	return nullptr;
}
//
class test_class_1: public php::class_base {
public:
	php::value method_1(php::parameters& params) {
		// 参数个数
		if(params.length() > 0)	return params[0];
		// 常见类型能够构造为 php::value 类型
		return params.length();
	}
	php::value method_2(php::parameters& params) {
		// 访问属性
		php::value p = prop("property_1");
		return p;
	}
	php::value method_3(php::parameters& params) {
		// 设置属性
		prop("property_1") = params[0];
		return params[0];
	}
};

php::value test_function_4(php::parameters& params) {
	return php::json_encode(params[0]);
}
php::value test_function_5(php::parameters& params) {
	return php::json_decode(params[0]);
}

void extension_init(php::extension_entry& extension) {
	// 扩展基本初始化 名称，版本
	extension.init(EXTENSION_NAME, EXTENSION_VERSION);
	// 全局或静态函数直接添加
	extension.add<test_function_1>("phpext_function_1");
	extension.add<test_function_2>("phpext_function_2");
	extension.add<test_function_3>("phpext_function_3", {
		// 参数说明可选（除 引用传递标识 true 外，实际不会有什么作用）
		php::of_string("arg_1", true),
	});
	extension.add<test_function_4>("phpext_function_4");
	extension.add<test_function_5>("phpext_function_5");
	php::class_entry<test_class_1> class_entry_1("phpext_class_1");
	// 类属性
	class_entry_1.add(php::property_entry("property_1", 123456));
	// 类成员函数需要加入 & 成员函数指针
	class_entry_1.add<&test_class_1::method_1>("method_1");
	class_entry_1.add<&test_class_1::method_2>("method_2");
	class_entry_1.add<&test_class_1::method_3>("method_3");
	// 将 类定义转入 扩展定义，注意 std::move 必须存在
	extension.add(std::move(class_entry_1));
}

