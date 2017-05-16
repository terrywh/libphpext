#pragma once
namespace php {
	void fail(const std::string& msg);
	void fail(const char *format, ...);
	void warn(const std::string& msg);
	void warn(const char *format, ...);
	void info(const std::string& msg);
	void info(const char *format, ...);

	const char* ini_get(char* name, uint name_len);

	inline void strtoupper_inplace(char* str, size_t len) {
		php_strtoupper(str, len);
	}
	inline void strtolower_inplace(char* str, size_t len) {
		php_strtolower(str, len);
	}
	php::string strtoupper(char* str, size_t len);
	php::string strtolower(char* str, size_t len);

	php::value make_exception(const std::string& message, int code = 0);
}
