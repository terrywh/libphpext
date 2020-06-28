<?php

var_dump(CONSTANT_1, CONSTANT_2);

echo cpp_hello("world"), "\n";
echo cpp_hello("wuhao"), "\n";

$x = 5;
echo $x, " + 5 = ", cpp_plus_5($x), " (", $x, ")\n";

echo cpp_container("key", new DateTime()), "\n";

echo cpp_invoke(function($who) {
    return "hello ".$who;
}), "\n";
