#include "phpext.h"

extern "C" {
	ZEND_DLEXPORT zend_module_entry* get_module() {
		static bool loaded = false;
		ZVAL_BOOL((zval*)&php::BOOL_TRUE, true);
		ZVAL_BOOL((zval*)&php::BOOL_FALSE, false);
		static php::extension_entry extension;
		if(!loaded) {
			extension_init(extension);
			extension.done();
			loaded = true;
		}
		return extension.entry();
	}
};
