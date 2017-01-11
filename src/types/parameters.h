#include <zend.h>
#include <cstdint>
#include "value.h"

#ifndef CORE_TYPES_PARAMETERS_H
#define CORE_TYPES_PARAMETERS_H

namespace core
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
				// value destructor will delref
				// so hereby addref to counter that
				Z_TRY_ADDREF_P(_argv + index);
				// copy = false to prevent value from malloc another zval
				return value(_argv + index, /*copy=*/false);
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


#endif // CORE_TYPES_PARAMETERS_H