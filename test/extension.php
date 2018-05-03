<?php
// 加载扩展
dl("phpext.so");

echo "test_function_1:", phpext_function_1("aaaaaaaa", 123), "\n";
echo "test_function_2:\n", phpext_function_2(function($array) {
	return count($array); // 计算回调数组长度
}), "\n";
$a = 123456; // 引用传递仅允许传递变量
echo "test_function_3:", phpext_function_3($a), $a, "\n";
echo "test_function_4:", $b = phpext_function_4(["a"=>"b"]), "\n";
echo "test_function_5:", var_export(phpext_function_5($b),true), "\n";
echo "test_function_6:", var_export(phpext_function_6(function() {
	echo "6";
})), "\n";
$obj = new phpext_class_1();
var_dump($obj);
echo "method_1:", $obj->method_1(), "\n";
echo "method_2:", $obj->method_2(), "\n";
echo "method_3:", $obj->method_3("bbbbbbbbbbbbb"), "\n";
echo "method_2:", $obj->method_2(), "\n";
var_dump($obj);
