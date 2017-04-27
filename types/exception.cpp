#include "../phpext.h"
namespace php {
	exception::exception(const std::string& message, int code)
		:_message(message), _code(code) {}
}
