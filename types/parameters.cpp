#include "../phpext.h"

namespace php {
	parameters::parameters(zend_execute_data* execute_data) {
		_size = ZEND_CALL_NUM_ARGS(execute_data);
		if(_size > 0) {
			_argv = ZEND_CALL_ARG(execute_data, 1);
		}
	}
	parameters::parameters(int argc, zval* argv)
	: _size(argc)
	, _argv(argv) {

	}
	parameters::parameters(std::vector<php::value>& argv)
	: _size(argv.size())
	, _argv(reinterpret_cast<zval*>(argv.data())) {

	}
	value& parameters::operator[](std::uint8_t index) {
		if(index >= _size) { // 不允许访问不存在的参数（模拟实现 PHP 内置的参数数量检查）
			std::string msg("parameter #");
			msg.append(std::to_string(index));
			msg.append(" is missing'");
			throw exception(msg, exception::PARAMETERS_INSUFFICIENT);
		}
		zval* arg = _argv + index;
		ZVAL_DEREF(arg);
		return *reinterpret_cast<value*>(arg);
	}
}
