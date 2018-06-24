#pragma once

#include "value.h"
#include "array.h"

namespace php {
	class string;
	array server();
	value server(const string& name);
	value constant(const string& name);
}