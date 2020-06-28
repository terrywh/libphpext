<?php
// 常量
var_dump(CPP_CONSTANT_1, CPP_CONSTANT_2);
// 一般函数
$w = "world";
echo cpp_hello("world"), "\n";
echo cpp_hello($w), "\n";

// 引用参数
$x = 5;
echo $x, " + 5 = ", cpp_plus_5($x), " (", $x, ")\n";
// C++ 容器
echo cpp_container("key", new DateTime()), "\n";
// 回调
echo cpp_invoke(function($who) {
    return "hello ".$who;
}), "\n";
// 数组（遍历）
echo cpp_walk(["a"=>1, "b" => $w, "c" => 3.0, "d" => new DateTime()]), "\n";
// 配置
var_dump(ini_get("example.hello"), ini_get("example.size"));
var_dump(cpp_conf_bytes("example.size"));