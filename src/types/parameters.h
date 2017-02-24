#pragma once

#include "../vendor.h"
#include "value.h"

namespace php
{

	class parameters
	{
		public:
			parameters(zend_execute_data* execute_data)
			{
				_size = ZEND_CALL_NUM_ARGS(execute_data);
				if(_size > 0) {
					_argv = ZEND_CALL_ARG(execute_data, 1);
				}
			}
			inline value operator[](std::uint8_t index) {
				assert(index < _size);
				// value 析构时会进行 Z_TRY_DELREF_P
				Z_TRY_ADDREF_P(_argv + index);
				// refer = true 时 value 不会进行额外的内存申请
				return value(_argv + index, /*refer=*/true);
			};
			inline std::uint8_t length()
			{
				return _size;
			}
		private:
			zval*        _argv;
			std::uint8_t _size;
	};
}
