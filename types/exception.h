#pragma once

namespace php {
	class value;
	class exception: public std::exception {
	public:
		enum {
			INVOKE_CALLABLE_FAILED  = -1000,
			INVOKE_METHOD_FAILED    = -1001,
			PARAMETERS_INSUFFICIENT = -1002,
		};
		exception(const std::string& message, int code = 0);
		inline const char* what() const noexcept {
			return _message.c_str();
		}
		inline const int code() const noexcept {
			return _code;
		}
		operator value() const;
	private:
		std::string  _message;
		int          _code;
	};
}
