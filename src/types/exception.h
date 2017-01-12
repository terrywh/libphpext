#include "../vendor.h"

#ifndef PHPEXT_TYPES_EXCEPTION_H
#define PHPEXT_TYPES_EXCEPTION_H

namespace phpext
{
namespace types
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
}}

#endif
