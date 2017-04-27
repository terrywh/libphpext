#include "../phpext.h"

namespace php {
	class_base::class_base() {
		ZVAL_UNDEF(&value_);
	}

	class_base::class_base(const class_base& base) {
		ZVAL_COPY(&value_, &base.value_);
	}
	class_base::class_base(class_base&& base) {
		ZVAL_COPY_VALUE(&value_, &base.value_);
		ZVAL_UNDEF(&base.value_);
	}

	value& class_base::prop(const char* name, std::size_t len) {
		zval defv;
		value* rv = (value*)zend_read_property(Z_OBJ(value_)->ce, &value_, name, len, true, &defv);
		return *rv;
	}
	value& class_base::prop(const std::string& name) {
		return prop(name.c_str(), name.length());
	}
}
