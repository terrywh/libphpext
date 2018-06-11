#pragma once

namespace php {
	class error {
	public:
		// 全局 PHP 错误
		// -----------------------------------------------------------------------
		static bool exists() {
			return PG(last_error_type) != 0;
		}
		static error_info ginfo() {
			return error_info();
		}
	};
}