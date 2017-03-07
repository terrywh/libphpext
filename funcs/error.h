#pragma once

namespace php {
	void fail(const char *format, va_list argv);
	void fail(const char *format);
	void warn(const char *format, va_list argv);
	void warn(const char *format);
	void info(const char *format, va_list argv);
	void info(const char *format);
}