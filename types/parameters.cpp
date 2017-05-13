#include "../phpext.h"

namespace php {
	parameters::parameters(zend_execute_data* execute_data) {
		_size = ZEND_CALL_NUM_ARGS(execute_data);
		if(_size > 0) {
			_argv = ZEND_CALL_ARG(execute_data, 1);
		}
	}
	parameters::parameters(int argc, zval argv[])
	: _size(argc)
	, _argv(argv) {

	}
	value& parameters::operator[](std::uint8_t index) {
		if(index >= _size) { // 不允许访问不存在的参数（模拟实现 PHP 内置的参数数量检查）
			throw exception("parameters missing", exception::PARAMETERS_INSUFFICIENT);
		}
		zval* arg = _argv + index;
		// ZVAL_DEREF(arg);
		return *reinterpret_cast<value*>(arg);
	}
}
