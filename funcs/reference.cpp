#include "../phpext.h"

namespace php {
	php::value ref(php::value& v) {
		php::value r;
		ZVAL_NEW_REF(static_cast<zval*>(r), static_cast<zval*>(v));
		v.addref(); // r -> ref (1) -> v (+1)
		return std::move(r);
	}
}
