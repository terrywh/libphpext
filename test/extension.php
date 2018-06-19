<?php
echo "========================================================\n";
echo "CONSTANT_1:", var_export(CONSTANT_1), "\n";
echo "CONSTANT_1:", var_export(CONSTANT_2), "\n";
echo "config_1: ", var_export(ini_get("config_1")), "\n";
echo "config_2: ", var_export(ini_get("config_2")), "\n";
echo "========================================================\n";
echo "test_function_1:\n";
echo "--------------------------------------------------------\n";
echo "    ", var_export(test_function_1(true, 123, 456.789, "abcdefg", ["a"=>"aaaaa"])), "\n";

test_function_2(function($abc) {
	echo "this is the callback being call from cpp: [", $abc, "]\n";
}, "abc");

test_function_2(function() {
	throw new Exception("exception_inside_callable");
}, "123");

$a = 123456; // 引用传递仅允许传递变量
echo "========================================================\n";
echo "test_function_3:\n";
echo "--------------------------------------------------------\n";
test_function_3($a);
echo var_export($a), "\n";
// 参数个数不符合要求
try{
	test_function_3();
}catch(TypeError $ex) {
	echo $ex, "\n";
}
// 基础类型检查“未强化”
$a = "123456";
test_function_3($a);
echo "========================================================\n";
echo "test_function_4:\n";
echo "--------------------------------------------------------\n";
// Object 指定类 或 Callable 会进行 调用前类型检查
test_function_4(new DateTime(), function($json) {
	echo $json, "\n";
});
try{
test_function_4(new DateTime(), 123);
}catch(TypeError $ex) {
	echo $ex, "\n";
}
try{
	test_function_4(123);
}catch(TypeError $ex) {
	echo $ex, "\n";
}
echo "========================================================\n";
echo "test_function_5:\n";
echo "--------------------------------------------------------\n";
$cb = test_function_5(5);
var_dump( $cb(3) );
echo "========================================================\n";
echo "test_class_1:\n";
echo "--------------------------------------------------------\n";
$obj = new test_class_1();
var_dump($obj);
echo "    method_1:\n";
$obj->method_1("this is the new value for property_1");
echo "    method_2:\n";
$obj->method_2("this is the new value for property_2");
var_dump($obj);
