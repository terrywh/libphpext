#pragma once

#include "error_info.h"
#include "value.h"

namespace php {
	class CLASS;
	class parameter;
	class property;
	class array_member;
	// exception => zend_ce_throwable
	class exception: public value, public std::exception {
	public:
		// 将 PHP 中发生的异常重新抛出到 CPP 中
		static void rethrow();
		// 将 CPP 中捕获的异常重新抛出到 PHP 中
		static void rethrow(const php::exception& ex);
		exception();
		exception(std::nullptr_t n);
		exception(zval* v, bool ref = false);
		exception(zend_object* v);
		exception(const parameter& v);
		exception(const property& v);
		exception(const array_member& v);
		exception(const CLASS& c, const std::string& message, int code = 0);
		exception(const value& v);
		exception(value&& v);
		// ----------------------------------------------------------------
		using value::operator =;
		// ----------------------------------------------------------------
		error_info info() const;
		virtual const char* what() const noexcept;
	};
}
