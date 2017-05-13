#include "../phpext.h"

namespace php {
	void array_merge(php::array& dest, php::array& src) {
		php_array_merge(static_cast<zend_array*>(dest), static_cast<zend_array*>(src));
	}
}
