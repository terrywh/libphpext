#include "../../core.h"

namespace phpext {
	namespace internal {
		constant_adder& constant_adder::add(const constant& c) {
			_constants.push_back(c);
			return *this;
		}
		void constant_adder::do_register_constants(int module) {
			while(!_constants.empty()) {
				zend_constant& c = _constants.front();
				c.module_number = module;
				zend_register_constant(&c);
				_constants.pop_front();
			}
		}
	}
}