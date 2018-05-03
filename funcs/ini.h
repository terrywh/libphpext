#pragma once
namespace php {
	php::string  ini_get_string(const std::string& name);
	std::int64_t ini_get_long(const std::string& name);
	double       ini_get_double(const std::string& name);
	bool         ini_get_boolean(const std::string& name);
	php::array   parse_ini_file(const std::string& filename);
}
