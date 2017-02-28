#pragma once

#include "../types/value.h"

namespace php {
	class class_base {
	public:
		class_base();
		class_base(const class_base& base);
		value prop(const char* name);
		class_base& prop(const char* name, const value& v);
		inline void _object_set(zend_object* obj) {
			ZVAL_OBJ(&val_, obj);
		}
		operator value() {
			std::printf("convert to value\n");
			return value(&val_);
		}
		static zend_class_entry* ce;
	private:
		zval val_;
	};
}
