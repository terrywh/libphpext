#include "phpext.h"

namespace php {
	array server() {
		php::array symbol(&EG(symbol_table));
		return symbol.get({"_SERVER", 7});
	}
	value server(const string& name) {
		return server().get(name);
	}
	value constant(const string& name) {
		return value(zend_get_constant_ex(name, NULL, ZEND_FETCH_CLASS_SILENT));
	}
}