#include "../phpext.h"

namespace php {
	parameters::parameters(zend_execute_data* execute_data) {
		_size = ZEND_CALL_NUM_ARGS(execute_data);
		if(_size > 0) {
			_argv = ZEND_CALL_ARG(execute_data, 1);
		}
	}
	value& parameters::operator[](std::uint8_t index) {
		if(index >= _size) { // 不允许访问不存在的参数（模拟实现 PHP 内置的参数数量检查）
			throw exception("parameters missing", exception::PARAMETERS_INSUFFICIENT);
		}
		zval* arg = _argv + index;
		if(Z_TYPE_P(arg) == IS_REFERENCE) {
			arg = Z_REFVAL_P(arg);
		}
		return *((value*)arg);
	}
}