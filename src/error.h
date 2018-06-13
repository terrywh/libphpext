#pragma once

namespace php {
	class error {
	public:
		// 全局 PHP 错误
		// -----------------------------------------------------------------------
		static bool exists();
		static error_info info();
	};
}