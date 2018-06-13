#pragma once

namespace php {
	enum : uint32_t {
		PUBLIC    = ZEND_ACC_PUBLIC,
		PROTECTED = ZEND_ACC_PROTECTED,
		PRIVATE   = ZEND_ACC_PROTECTED,
	};
}