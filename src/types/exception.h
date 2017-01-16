#pragma once

#include "../vendor.h"

namespace php
{
	class exception: public std::exception {
	public:
		exception(const std::string& message, std::int64_t code = 0)
		:_message(message) {

		}
		virtual const char* what() const noexcept {
			return _message.c_str();
		}
		virtual const std::int64_t code() const noexcept {
			return _code;
		}
	private:
		std::string  _message;
		std::int64_t _code;
	};
}
