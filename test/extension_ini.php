<?php
dl("phpext.so");
phpinfo(INFO_MODULES);
// function callback1() {
// 	return 1;
// }
// $start = microtime(true);
// for($i=0;$i<10000000;++$i) {
// 	$rst = core_test\test_function_1("callback1");
// }
// $end = microtime(true);
// echo "[", $rst, "] ", $i / ($end - $start), "/s\n";
// // --------------------------------
// $start = $end;
// for($i=0;$i<10000000;++$i) {
// 	$rst = core_test\test_function_1(function() {
// 		return 2;
// 	});
// }
// $end = microtime(true);
// echo "[", $rst, "] ", $i / ($end - $start), "/s\n";
// // --------------------------------
// $cb = function() {
// 	return 3;
// };
// $start = $end;
// for($i=0;$i<10000000;++$i) {
// 	$rst = core_test\test_function_1($cb);
// }
// $end = microtime(true);
// echo "[", $rst, "] ", $i / ($end - $start), "/s\n";
