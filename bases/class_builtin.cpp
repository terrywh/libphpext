#include "../phpext.h"

namespace php {
	value class_closure::__invoke(parameters& params) {
		return fn_(params);
	}
	void _register_builtin_class(extension_entry& extension) {
		php::class_entry<php::class_closure> class_closure_entry("__closure");
		class_closure_entry.add<&php::class_closure::__invoke>("__invoke");
		extension.add(std::move(class_closure_entry));
	}

}
