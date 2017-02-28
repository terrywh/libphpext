#pragma once

#include "../vendor.h"

namespace php
{

	class exception: public std::exception {
	public:
		enum {
			INVOKE_CALLABLE_FAILED  = -1000,
			INVOKE_METHOD_FAILED    = -1001,
			PARAMETERS_INSUFFICIENT = -1002,
		};
		exception(const std::string& message, int code = 0)
		:_message(message) {

		}
		virtual const char* what() const noexcept {
			return _message.c_str();
		}
		virtual const int code() const noexcept {
			return _code;
		}
	private:
		std::string  _message;
		std::int64_t _code;
	};
}
