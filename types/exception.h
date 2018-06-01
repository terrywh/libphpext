#pragma once

namespace php {
	class exception: public php::value {
	private:
		class exception_previous {
		public:
			exception_previous(exception& ex);
			~exception_previous();
		private:
			exception&   ex_;
			zval       prev_;
		};
		exception() {
			// ZVAL_NULL(&value_);
		}
	public:
		enum {
			INVOKE_CALLABLE_FAILED  = -1000,
			INVOKE_METHOD_FAILED    = -1001,
			PARAMETERS_INSUFFICIENT = -1002,
		};
		// 全局 PHP 异常操作
		// 是否存在异常
		static bool has();
		// 获取异常对象(的拷贝)
		static exception get();
		// 清理 PHP 的异常状态
		static void off();
		static exception create(const php::string& message, int code = 0) {
			return exception(message, code);
		}
		static exception create(const char* message, size_t size = -1, int code = 0) {
			return exception(php::string(message, (size == -1 ? std::strlen(message) : size)), code);
		}
		explicit exception(const php::string& message, int code = 0);
		explicit exception(const std::string& message, int code = 0)
		: exception(php::string(message), code) {}
		php::string message() const noexcept;
		php::string file() const noexcept;
		int line() const noexcept;
		int code() const noexcept;

		bool global();

	};
}
