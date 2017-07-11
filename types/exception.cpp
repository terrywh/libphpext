#include "../phpext.h"
namespace php {
	exception::exception(const std::string& message, int code)
		:_message(message), _code(code) {}
	exception::operator value() const {
		return make_exception(_message, _code);
	}
}
