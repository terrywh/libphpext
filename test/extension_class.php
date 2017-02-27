<?php
dl("phpext.so");
// var_dump(get_declared_classes());
echo "class_exists('phpext_class_1') => ", (class_exists("phpext_class_1") ? "true" : "false"), "\n";

$obj = new phpext_class_1();
var_dump($obj);
echo 'method_1("aaaaa") => ', $obj->method_1("aaaaa"), "\n";
echo 'method_2() => ', $obj->method_2(), "\n";
$a = "aaaaaa";
echo 'method_3($a = "aaaaaa") => ', $obj->method_3($a), ' $a => ', $a, "\n";
