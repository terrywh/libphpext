#pragma once

namespace php {
	// exception => zend_ce_throwable
	class exception: public value, public std::exception {
	protected:
		exception();
	public:
		// 将 PHP 中发生的异常重新抛出到 CPP 中
		static void rethrow();
		// 将 CPP 中捕获的异常重新抛出到 PHP 中
		static void rethrow(const php::exception& ex);
		// 注意: 此种构造形式无类型检查
		explicit exception(const zval* v);
		exception(zend_object* v);
		exception(const CLASS& c, const std::string& message, int code = 0);
		exception(const value& v);
		exception(value&& v);
		// ----------------------------------------------------------------
		exception& operator =(const value& v);
		exception& operator =(value&& v);
		// ----------------------------------------------------------------
		error_info info() const;
		virtual const char* what() const noexcept;
	};
}
