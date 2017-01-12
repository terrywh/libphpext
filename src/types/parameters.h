#include "../vendor.h"
#include "value.h"

#ifndef PHPEXT_TYPES_PARAMETERS_H
#define PHPEXT_TYPES_PARAMETERS_H

namespace phpext
{
namespace types
{

	class parameters
	{
		public:
			parameters()
			{
				_size = ZEND_CALL_NUM_ARGS(EG(current_execute_data));
				if(_size > 0) {
					_argv = ZEND_CALL_ARG(EG(current_execute_data), 1);
				}
			}
			inline value operator[](std::uint8_t index)
			{
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
}}


#endif // PHPEXT_TYPES_PARAMETERS_H
