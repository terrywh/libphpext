<?php
// 常量
var_dump(CPP_CONSTANT_1, CPP_CONSTANT_2);
// 一般函数
echo "--> hello:\n";
echo cpp_hello(), "\n";
// 引用参数
echo "--> reference:\n";
$x = 5;
echo $x, " + 5 = ", cpp_plus_5($x), " (", $x, ")\n";
// C++ 容器
echo "--> cpp container:\n";
echo cpp_container("key", new DateTime()), "\n";
// 回调
echo "--> callback (cpp -> php):\n";
echo cpp_invoke(function($who) {
    return "hello ".$who;
}), "\n";
// 数组（遍历）
echo "--> array walk:\n";
echo cpp_walk(["a"=>1, "b" => "world", "c" => 3.0, "d" => new DateTime()]), "\n";
// 配置
echo "--> ini entry:\n";
var_dump(ini_get("example.hello"), ini_get("example.size"));
var_dump(cpp_conf_bytes("example.size"));
// 属性访问
echo "--> property:\n";
$x = DateInterval::createFromDateString("1 day");
var_dump(cpp_property($x), $x->d);

// 类常量
echo "--> class const:\n";
var_dump(cpp_example::CONST_1);
// 对象
echo "--> class object\n";
$x = new cpp_example();
var_dump($x);
// 方法
echo "--> method:\n";
echo $x->hello("world"), "\n"; // 普通成员方法
var_dump($x instanceof Countable, count($x)); // 实现接口方法
var_dump(cpp_example::number()); // 静态方法
echo "--> property:\n";
var_dump($x->prop1); // 普通属性
var_dump(cpp_example::$prop2); // 静态属性
var_dump($x->prop3); // 同步属性

echo "--> attribute:\n";
#[Attribute]
class dummy {
    function __construct($data) {
        echo $data, "\n";
    }
}

#[dummy("hello")]
#[cpp_attribute("GET", "/hello")]
function hello() {
    $r = new ReflectionFunction("hello");
    $r->getAttributes("dummy")[0]->newInstance();
    $r->getAttributes("cpp_attribute")[0]->newInstance();
}
hello();

$r = new ReflectionClass("cpp_example");
$r->getAttributes("cpp_attribute")[0]->newInstance();
