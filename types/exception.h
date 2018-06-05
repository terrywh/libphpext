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
		// 全局 PHP 异常状态数据
		static bool has();
		static exception get();
		static void off();
		static exception create(const php::string& message, int code = 0) {
			return exception(message, code);
		}
		explicit exception(const php::string& message, int code = 0);
		php::string message() const noexcept;
		php::string file() const noexcept;
		int line() const noexcept;
		int code() const noexcept;

		bool global();

	};
}
