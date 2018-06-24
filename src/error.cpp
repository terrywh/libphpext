#include "vendor.h"
#include "error.h"

namespace php {
	// 全局 PHP 错误
	// -----------------------------------------------------------------------
	bool error::exists() {
		return PG(last_error_type) != 0;
	}
	error_info error::info() {
		return error_info();
	}
}