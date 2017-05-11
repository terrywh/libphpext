#pragma once
namespace php {
	void fail(const std::string& msg);
	void fail(const char *format, ...);
	void warn(const std::string& msg);
	void warn(const char *format, ...);
	void info(const std::string& msg);
	void info(const char *format, ...);

	const char* ini_get(char* name, uint name_len);

	inline void strtoupper(char* s, size_t len) {
		php_strtoupper(s, len);
	}
	inline void strtolower(char* s, size_t len) {
		php_strtolower(s, len);
	}
}
