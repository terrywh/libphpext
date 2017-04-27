#pragma once

namespace php {
	class parameters {
		public:
			parameters(zend_execute_data* execute_data);
			value& operator[](std::uint8_t index);
			inline std::uint8_t length() const {
				return _size;
			}
		private:
			zval*        _argv;
			std::uint8_t _size;
	};
}
