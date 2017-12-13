#include "../phpext.h"
namespace php {
	exception::exception(const std::string& message, int code)
		:_message(message), _code(code) {}
	exception::operator value() const {
		php::object ex = php::object::create_exception(_message, _code);
		return std::move(ex);
	}
}
