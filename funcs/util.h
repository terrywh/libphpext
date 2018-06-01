#pragma once
namespace php {
	void fail(const std::string& msg);
	void fail(const char *format, ...);
	void warn(const std::string& msg);
	void warn(const char *format, ...);
	void info(const std::string& msg);
	void info(const char *format, ...);
	inline void strtoupper_inplace(char* str, size_t len) {
		php_strtoupper(str, len);
	}
	inline void strtolower_inplace(char* str, size_t len) {
		php_strtolower(str, len);
	}
	php::string strtoupper(char* str, size_t len);
	php::string strtolower(char* str, size_t len);
	// 在 shutdown 前执行回调 (相对 module_shutdown 更早, 在 php/zend 引擎销毁前)
	const char* error_type_name(int type);
	void register_shutdown_function(const php::callable& cb);
}
