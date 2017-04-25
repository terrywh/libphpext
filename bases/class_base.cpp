#include "../phpext.h"

namespace php {
	class_base::class_base() {
		ZVAL_UNDEF(&val_);
	}

	class_base::class_base(const class_base& base) {
		// 注意：val_ 字段不要初始化（等待 _object_set）
	}

	value class_base::prop(const char* name, std::size_t len) {
		zval rv;
		return value( zend_read_property(Z_OBJ(val_)->ce, &val_, name, len, true, &rv), true );
	}

	value class_base::prop(const std::string& name) {
		return prop(name.c_str(), name.length());
	}
}
