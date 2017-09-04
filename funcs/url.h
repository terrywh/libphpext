#pragma once
namespace php {
	std::shared_ptr<php_url> parse_url(const char* url, std::size_t url_len);
	php::string build_query(php::array array, const std::string& prefix="");
	php::array  parse_query(const char sep, const char* buffer, std::size_t n);
}
