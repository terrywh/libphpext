#include "phpext.h"

extern "C" {
	ZEND_DLEXPORT zend_module_entry* get_module() {
		static bool loaded = false;
		static php::extension_entry extension;
		if(!loaded) {
			extension_init(extension);
			extension.done();
			loaded = true;
		}
		return extension.entry();
	}
};
