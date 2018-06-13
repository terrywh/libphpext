#pragma once

namespace php {
	array server();
	value server(const string& name);
	value constant(const string& name);
}