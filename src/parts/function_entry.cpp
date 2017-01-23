#include "function_entry.h"

namespace php {
	function_entry::function_entry(const std::string& name, value (*handler)(parameters& params))
	:name_(name), handler_(handler) {}
}
