<?php

function dump($array) {
    $count = 0;
    foreach($array as $key => $val) {
        echo $key, " => ", (is_string($val) ? $val : json_encode($val, JSON_UNESCAPED_UNICODE)), "\n";
        ++$count;
    }
    return $count;
}
var_dump(CPP_CONSTANT_1, CPP_CONSTANT_2);

$w = "world";
echo cpp_hello("world"), "\n";
echo cpp_hello($w), "\n";

$x = 5;
echo $x, " + 5 = ", cpp_plus_5($x), " (", $x, ")\n";

echo cpp_container("key", new DateTime()), "\n";

echo cpp_invoke(function($who) {
    return "hello ".$who;
}), "\n";

// echo dump(["a"=>1, "b" => "222222222222222222222222222", "c" => 3.0, "d" => new DateTime()]), "\n";
echo cpp_walk(["a"=>1, "b" => $w, "c" => 3.0, "d" => new DateTime()]), "\n";