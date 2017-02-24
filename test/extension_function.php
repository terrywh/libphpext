<?php
dl("phpext.so");

echo 'phpext_function_1() => ', phpext_function_1(), "\n";

function callback($data) {
	echo 'callback( ', $data, ' )',"\n";
	return 123456;
}

echo 'phpext_function_2("callback") => ', phpext_function_2("callback"), "\n";
$a = "aaaaa";
echo 'phpext_function_3($a = "aaaaa") => ', phpext_function_3($a), ' $a => ', $a, "\n";
