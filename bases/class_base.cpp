#include "../phpext.h"

namespace php {
	class_base::class_base() {
		ZVAL_UNDEF(&val_);
	}

	class_base::class_base(const class_base& base) {
		// 注意：val_ 字段不要初始化（等待 _object_set）
	}

	value class_base::prop(const char* name) {
		zval rv;
		return value( zend_read_property(Z_OBJ(val_)->ce, &val_, name, std::strlen(name), true, &rv) );
	}

	class_base& class_base::prop(const char* name, const value& v) {
		zend_update_property(Z_OBJ(val_)->ce, &val_, name, std::strlen(name), v.data());
		return *this;
	}
}
